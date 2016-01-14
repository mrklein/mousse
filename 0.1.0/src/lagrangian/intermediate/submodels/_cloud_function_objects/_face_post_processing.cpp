// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_face_post_processing.hpp"
#include "pstream.hpp"
#include "list_list_ops.hpp"
#include "surface_writer.hpp"
#include "global_index.hpp"
// Private Member Functions 
template<class CloudType>
void mousse::FacePostProcessing<CloudType>::makeLogFile
(
  const word& zoneName,
  const label zoneI,
  const label nFaces,
  const scalar totArea
)
{
  // Create the output file if not already created
  if (log_)
  {
    if (debug)
    {
      Info<< "Creating output file." << endl;
    }
    if (Pstream::master())
    {
      // Create directory if does not exist
      mkDir(this->outputTimeDir());
      // Open new file at start up
      outputFilePtr_.set
      (
        zoneI,
        new OFstream
        (
          this->outputTimeDir()/(type() + '_' + zoneName + ".dat")
        )
      );
      outputFilePtr_[zoneI]
        << "# Source    : " << type() << nl
        << "# Face zone : " << zoneName << nl
        << "# Faces     : " << nFaces << nl
        << "# Area      : " << totArea << nl
        << "# Time" << tab << "mass" << tab << "massFlowRate" << endl;
    }
  }
}
template<class CloudType>
void mousse::FacePostProcessing<CloudType>::write()
{
  const fvMesh& mesh = this->owner().mesh();
  const Time& time = mesh.time();
  const faceZoneMesh& fzm = mesh.faceZones();
  scalar timeNew = time.value();
  scalar timeElapsed = timeNew - timeOld_;
  totalTime_ += timeElapsed;
  const scalar alpha = (totalTime_ - timeElapsed)/totalTime_;
  const scalar beta = timeElapsed/totalTime_;
  FOR_ALL(faceZoneIDs_, zoneI)
  {
    massFlowRate_[zoneI] =
      alpha*massFlowRate_[zoneI] + beta*mass_[zoneI]/timeElapsed;
    massTotal_[zoneI] += mass_[zoneI];
  }
  const label procI = Pstream::myProcNo();
  Info<< type() << " output:" << nl;
  List<scalarField> zoneMassTotal(mass_.size());
  List<scalarField> zoneMassFlowRate(massFlowRate_.size());
  FOR_ALL(faceZoneIDs_, zoneI)
  {
    const word& zoneName = fzm[faceZoneIDs_[zoneI]].name();
    scalarListList allProcMass(Pstream::nProcs());
    allProcMass[procI] = massTotal_[zoneI];
    Pstream::gatherList(allProcMass);
    zoneMassTotal[zoneI] =
      ListListOps::combine<scalarList>
      (
        allProcMass, accessOp<scalarList>()
      );
    const scalar sumMassTotal = sum(zoneMassTotal[zoneI]);
    scalarListList allProcMassFlowRate(Pstream::nProcs());
    allProcMassFlowRate[procI] = massFlowRate_[zoneI];
    Pstream::gatherList(allProcMassFlowRate);
    zoneMassFlowRate[zoneI] =
      ListListOps::combine<scalarList>
      (
        allProcMassFlowRate, accessOp<scalarList>()
      );
    const scalar sumMassFlowRate = sum(zoneMassFlowRate[zoneI]);
    Info<< "    " << zoneName
      << ": total mass = " << sumMassTotal
      << "; average mass flow rate = " << sumMassFlowRate
      << nl;
    if (outputFilePtr_.set(zoneI))
    {
      OFstream& os = outputFilePtr_[zoneI];
      os  << time.timeName() << token::TAB << sumMassTotal << token::TAB
        << sumMassFlowRate<< endl;
    }
  }
  Info<< endl;
  if (surfaceFormat_ != "none")
  {
    FOR_ALL(faceZoneIDs_, zoneI)
    {
      const faceZone& fZone = fzm[faceZoneIDs_[zoneI]];
      labelList pointToGlobal;
      labelList uniqueMeshPointLabels;
      autoPtr<globalIndex> globalPointsPtr =
        mesh.globalData().mergePoints
        (
          fZone().meshPoints(),
          fZone().meshPointMap(),
          pointToGlobal,
          uniqueMeshPointLabels
        );
      pointField uniquePoints(mesh.points(), uniqueMeshPointLabels);
      List<pointField> allProcPoints(Pstream::nProcs());
      allProcPoints[procI] = uniquePoints;
      Pstream::gatherList(allProcPoints);
      faceList faces(fZone().localFaces());
      FOR_ALL(faces, i)
      {
        inplaceRenumber(pointToGlobal, faces[i]);
      }
      List<faceList> allProcFaces(Pstream::nProcs());
      allProcFaces[procI] = faces;
      Pstream::gatherList(allProcFaces);
      if (Pstream::master())
      {
        pointField allPoints
        (
          ListListOps::combine<pointField>
          (
            allProcPoints, accessOp<pointField>()
          )
        );
        faceList allFaces
        (
          ListListOps::combine<faceList>
          (
            allProcFaces, accessOp<faceList>()
          )
        );
        autoPtr<surfaceWriter> writer
        (
          surfaceWriter::New
          (
            surfaceFormat_,
            this->coeffDict().subOrEmptyDict("formatOptions").
              subOrEmptyDict(surfaceFormat_)
          )
        );
        writer->write
        (
          this->outputTimeDir(),
          fZone.name(),
          allPoints,
          allFaces,
          "massTotal",
          zoneMassTotal[zoneI],
          false
        );
        writer->write
        (
          this->outputTimeDir(),
          fZone.name(),
          allPoints,
          allFaces,
          "massFlowRate",
          zoneMassFlowRate[zoneI],
          false
        );
      }
    }
  }
  if (resetOnWrite_)
  {
    FOR_ALL(faceZoneIDs_, zoneI)
    {
      massFlowRate_[zoneI] = 0.0;
    }
    timeOld_ = timeNew;
    totalTime_ = 0.0;
  }
  FOR_ALL(mass_, zoneI)
  {
    mass_[zoneI] = 0.0;
  }
  // writeProperties();
}
// Constructors 
template<class CloudType>
mousse::FacePostProcessing<CloudType>::FacePostProcessing
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
  faceZoneIDs_(),
  surfaceFormat_(this->coeffDict().lookup("surfaceFormat")),
  resetOnWrite_(this->coeffDict().lookup("resetOnWrite")),
  totalTime_(0.0),
  mass_(),
  massTotal_(),
  massFlowRate_(),
  log_(this->coeffDict().lookup("log")),
  outputFilePtr_(),
  timeOld_(owner.mesh().time().value())
{
  wordList faceZoneNames(this->coeffDict().lookup("faceZones"));
  mass_.setSize(faceZoneNames.size());
  massTotal_.setSize(faceZoneNames.size());
  massFlowRate_.setSize(faceZoneNames.size());
  outputFilePtr_.setSize(faceZoneNames.size());
  DynamicList<label> zoneIDs;
  const faceZoneMesh& fzm = owner.mesh().faceZones();
  const surfaceScalarField& magSf = owner.mesh().magSf();
  const polyBoundaryMesh& pbm = owner.mesh().boundaryMesh();
  FOR_ALL(faceZoneNames, i)
  {
    const word& zoneName = faceZoneNames[i];
    label zoneI = fzm.findZoneID(zoneName);
    if (zoneI != -1)
    {
      zoneIDs.append(zoneI);
      const faceZone& fz = fzm[zoneI];
      mass_[i].setSize(fz.size(), 0.0);
      massTotal_[i].setSize(fz.size(), 0.0);
      massFlowRate_[i].setSize(fz.size(), 0.0);
      label nFaces = returnReduce(fz.size(), sumOp<label>());
      Info<< "        " << zoneName << " faces: " << nFaces << nl;
      scalar totArea = 0.0;
      FOR_ALL(fz, j)
      {
        label faceI = fz[j];
        if (faceI < owner.mesh().nInternalFaces())
        {
          totArea += magSf[fz[j]];
        }
        else
        {
          label bFaceI = faceI - owner.mesh().nInternalFaces();
          label patchI = pbm.patchID()[bFaceI];
          const polyPatch& pp = pbm[patchI];
          if
          (
            !magSf.boundaryField()[patchI].coupled()
          || refCast<const coupledPolyPatch>(pp).owner()
          )
          {
            label localFaceI = pp.whichFace(faceI);
            totArea += magSf.boundaryField()[patchI][localFaceI];
          }
        }
      }
      totArea = returnReduce(totArea, sumOp<scalar>());
      makeLogFile(zoneName, i, nFaces, totArea);
    }
  }
  faceZoneIDs_.transfer(zoneIDs);
  // readProperties(); AND initialise mass... fields
}
template<class CloudType>
mousse::FacePostProcessing<CloudType>::FacePostProcessing
(
  const FacePostProcessing<CloudType>& pff
)
:
  CloudFunctionObject<CloudType>(pff),
  faceZoneIDs_(pff.faceZoneIDs_),
  surfaceFormat_(pff.surfaceFormat_),
  resetOnWrite_(pff.resetOnWrite_),
  totalTime_(pff.totalTime_),
  mass_(pff.mass_),
  massTotal_(pff.massTotal_),
  massFlowRate_(pff.massFlowRate_),
  log_(pff.log_),
  outputFilePtr_(),
  timeOld_(0.0)
{}
// Destructor 
template<class CloudType>
mousse::FacePostProcessing<CloudType>::~FacePostProcessing()
{}
// Member Functions 
template<class CloudType>
void mousse::FacePostProcessing<CloudType>::postFace
(
  const parcelType& p,
  const label faceI,
  bool&
)
{
  if
  (
    this->owner().solution().output()
  || this->owner().solution().transient()
  )
  {
    const faceZoneMesh& fzm = this->owner().mesh().faceZones();
    FOR_ALL(faceZoneIDs_, i)
    {
      const faceZone& fz = fzm[faceZoneIDs_[i]];
      label faceId = -1;
      FOR_ALL(fz, j)
      {
        if (fz[j] == faceI)
        {
          faceId = j;
          break;
        }
      }
      if (faceId != -1)
      {
        mass_[i][faceId] += p.mass()*p.nParticle();
      }
    }
  }
}
