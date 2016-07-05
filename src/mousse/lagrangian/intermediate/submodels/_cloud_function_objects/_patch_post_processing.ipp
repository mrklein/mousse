// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_patch_post_processing.hpp"
#include "pstream.hpp"
#include "string_list_ops.hpp"
#include "list_ops.hpp"
#include "list_list_ops.hpp"


// Private Member Functions 
template<class CloudType>
mousse::label mousse::PatchPostProcessing<CloudType>::applyToPatch
(
  const label globalPatchI
) const
{
  FOR_ALL(patchIDs_, i) {
    if (patchIDs_[i] == globalPatchI) {
      return i;
    }
  }
  return -1;
}


// protected Member Functions 
template<class CloudType>
void mousse::PatchPostProcessing<CloudType>::write()
{
  FOR_ALL(patchData_, i) {
    List<List<scalar>> procTimes{Pstream::nProcs()};
    procTimes[Pstream::myProcNo()] = times_[i];
    Pstream::gatherList(procTimes);
    List<List<string>> procData{Pstream::nProcs()};
    procData[Pstream::myProcNo()] = patchData_[i];
    Pstream::gatherList(procData);
    if (Pstream::master()) {
      const fvMesh& mesh = this->owner().mesh();
      // Create directory if it doesn't exist
      mkDir(this->outputTimeDir());
      const word& patchName = mesh.boundaryMesh()[patchIDs_[i]].name();
      OFstream patchOutFile
      {
        this->outputTimeDir()/patchName + ".post",
        IOstream::ASCII,
        IOstream::currentVersion,
        mesh.time().writeCompression()
      };
      List<string> globalData;
      globalData =
        ListListOps::combine<List<string>>
        (
          procData,
          accessOp<List<string>>()
        );
      List<scalar> globalTimes;
      globalTimes =
        ListListOps::combine<List<scalar>>
        (
          procTimes,
          accessOp<List<scalar>>{}
        );
      labelList indices;
      sortedOrder(globalTimes, indices);
      string header{"# Time currentProc " + parcelType::propertyList_};
      patchOutFile << header.c_str() << nl;
      FOR_ALL(globalTimes, i) {
        label dataI = indices[i];
        patchOutFile
          << globalTimes[dataI] << ' '
          << globalData[dataI].c_str()
          << nl;
      }
    }
    patchData_[i].clearStorage();
    times_[i].clearStorage();
  }
}


// Constructors 
template<class CloudType>
mousse::PatchPostProcessing<CloudType>::PatchPostProcessing
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>{dict, owner, modelName, typeName},
  maxStoredParcels_{readScalar(this->coeffDict().lookup("maxStoredParcels"))},
  patchIDs_{},
  times_{},
  patchData_{}
{
  const wordList allPatchNames = owner.mesh().boundaryMesh().names();
  wordList patchName{this->coeffDict().lookup("patches")};
  labelHashSet uniquePatchIDs;
  FOR_ALL_REVERSE(patchName, i) {
    labelList patchIDs = findStrings(patchName[i], allPatchNames);
    if (patchIDs.empty()) {
      WARNING_IN
      (
        "mousse::PatchPostProcessing<CloudType>::PatchPostProcessing"
        "("
        "  const dictionary&, "
        "  CloudType& "
        ")"
      )
      << "Cannot find any patch names matching " << patchName[i]
      << endl;
    }
    uniquePatchIDs.insert(patchIDs);
  }
  patchIDs_ = uniquePatchIDs.toc();
  if (debug) {
    FOR_ALL(patchIDs_, i) {
      const label patchI = patchIDs_[i];
      const word& patchName = owner.mesh().boundaryMesh()[patchI].name();
      Info << "Post-process patch " << patchName << endl;
    }
  }
  patchData_.setSize(patchIDs_.size());
  times_.setSize(patchIDs_.size());
}


template<class CloudType>
mousse::PatchPostProcessing<CloudType>::PatchPostProcessing
(
  const PatchPostProcessing<CloudType>& ppm
)
:
  CloudFunctionObject<CloudType>{ppm},
  maxStoredParcels_{ppm.maxStoredParcels_},
  patchIDs_{ppm.patchIDs_},
  times_{ppm.times_},
  patchData_{ppm.patchData_}
{}


// Destructor 
template<class CloudType>
mousse::PatchPostProcessing<CloudType>::~PatchPostProcessing()
{}


// Member Functions 
template<class CloudType>
void mousse::PatchPostProcessing<CloudType>::postPatch
(
  const parcelType& p,
  const polyPatch& pp,
  const scalar,
  const tetIndices& /*tetIs*/,
  bool&
)
{
  const label patchI = pp.index();
  const label localPatchI = applyToPatch(patchI);
  if (localPatchI != -1 && patchData_[localPatchI].size() < maxStoredParcels_) {
    times_[localPatchI].append(this->owner().time().value());
    OStringStream data;
    data << Pstream::myProcNo() << ' ' << p;
    patchData_[localPatchI].append(data.str());
  }
}
