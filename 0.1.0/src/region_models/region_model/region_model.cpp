// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "mapped_wall_poly_patch.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Static Data Members
namespace mousse
{
namespace regionModels
{
  defineTypeNameAndDebug(regionModel, 0);
}
}
// Private Member Functions 
void mousse::regionModels::regionModel::constructMeshObjects()
{
  // construct region mesh
  if (!time_.foundObject<fvMesh>(regionName_))
  {
    regionMeshPtr_.reset
    (
      new fvMesh
      (
        IOobject
        (
          regionName_,
          time_.timeName(),
          time_,
          IOobject::MUST_READ
        )
      )
    );
  }
}
void mousse::regionModels::regionModel::constructMeshObjects
(
  const dictionary& dict
)
{
  // construct region mesh
  if (!time_.foundObject<fvMesh>(regionName_))
  {
    regionMeshPtr_.reset
    (
      new fvMesh
      (
        IOobject
        (
          regionName_,
          time_.timeName(),
          time_,
          IOobject::MUST_READ
        )
      )
    );
  }
}
void mousse::regionModels::regionModel::initialise()
{
  if (debug)
  {
    Pout<< "regionModel::initialise()" << endl;
  }
  label nBoundaryFaces = 0;
  DynamicList<label> primaryPatchIDs;
  DynamicList<label> intCoupledPatchIDs;
  const polyBoundaryMesh& rbm = regionMesh().boundaryMesh();
  forAll(rbm, patchI)
  {
    const polyPatch& regionPatch = rbm[patchI];
    if (isA<mappedPatchBase>(regionPatch))
    {
      if (debug)
      {
        Pout<< "found " << mappedWallPolyPatch::typeName
          <<  " " << regionPatch.name() << endl;
      }
      intCoupledPatchIDs.append(patchI);
      nBoundaryFaces += regionPatch.faceCells().size();
      const mappedPatchBase& mapPatch =
        refCast<const mappedPatchBase>(regionPatch);
      if
      (
        primaryMesh_.time().foundObject<polyMesh>
        (
          mapPatch.sampleRegion()
        )
      )
      {
        const label primaryPatchI = mapPatch.samplePolyPatch().index();
        primaryPatchIDs.append(primaryPatchI);
      }
    }
  }
  primaryPatchIDs_.transfer(primaryPatchIDs);
  intCoupledPatchIDs_.transfer(intCoupledPatchIDs);
  if (returnReduce(nBoundaryFaces, sumOp<label>()) == 0)
  {
    WarningIn("regionModel::initialise()")
      << "Region model has no mapped boundary conditions - transfer "
      << "between regions will not be possible" << endl;
  }
  if (!outputPropertiesPtr_.valid())
  {
    const fileName uniformPath(word("uniform")/"regionModels");
    outputPropertiesPtr_.reset
    (
      new IOdictionary
      (
        IOobject
        (
          regionName_ + "OutputProperties",
          time_.timeName(),
          uniformPath/regionName_,
          primaryMesh_,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE
        )
      )
    );
  }
}
// Protected Member Functions 
bool mousse::regionModels::regionModel::read()
{
  if (regIOobject::read())
  {
    if (active_)
    {
      if (const dictionary* dictPtr = subDictPtr(modelName_ + "Coeffs"))
      {
        coeffs_ <<= *dictPtr;
      }
      infoOutput_.readIfPresent("infoOutput", *this);
    }
    return true;
  }
  else
  {
    return false;
  }
}
bool mousse::regionModels::regionModel::read(const dictionary& dict)
{
  if (active_)
  {
    if (const dictionary* dictPtr = dict.subDictPtr(modelName_ + "Coeffs"))
    {
      coeffs_ <<= *dictPtr;
    }
    infoOutput_.readIfPresent("infoOutput", dict);
    return true;
  }
  else
  {
    return false;
  }
}
const mousse::AMIPatchToPatchInterpolation&
mousse::regionModels::regionModel::interRegionAMI
(
  const regionModel& nbrRegion,
  const label regionPatchI,
  const label nbrPatchI,
  const bool flip
) const
{
  label nbrRegionID = findIndex(interRegionAMINames_, nbrRegion.name());
  const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();
  if (nbrRegionID != -1)
  {
    if (!interRegionAMI_[nbrRegionID].set(regionPatchI))
    {
      const polyPatch& p = regionMesh().boundaryMesh()[regionPatchI];
      const polyPatch& nbrP = nbrRegionMesh.boundaryMesh()[nbrPatchI];
      int oldTag = UPstream::msgType();
      UPstream::msgType() = oldTag + 1;
      interRegionAMI_[nbrRegionID].set
      (
        regionPatchI,
        new AMIPatchToPatchInterpolation
        (
          p,
          nbrP,
          faceAreaIntersect::tmMesh,
          true,
          AMIPatchToPatchInterpolation::imFaceAreaWeight,
          -1,
          flip
        )
      );
      UPstream::msgType() = oldTag;
    }
    return interRegionAMI_[nbrRegionID][regionPatchI];
  }
  else
  {
    label nbrRegionID = interRegionAMINames_.size();
    interRegionAMINames_.append(nbrRegion.name());
    const polyPatch& p = regionMesh().boundaryMesh()[regionPatchI];
    const polyPatch& nbrP = nbrRegionMesh.boundaryMesh()[nbrPatchI];
    label nPatch = regionMesh().boundaryMesh().size();
    interRegionAMI_.resize(nbrRegionID + 1);
    interRegionAMI_.set
    (
      nbrRegionID,
      new PtrList<AMIPatchToPatchInterpolation>(nPatch)
    );
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag + 1;
    interRegionAMI_[nbrRegionID].set
    (
      regionPatchI,
      new AMIPatchToPatchInterpolation
      (
        p,
        nbrP,
        faceAreaIntersect::tmMesh,
        true,
        AMIPatchToPatchInterpolation::imFaceAreaWeight,
        -1,
        flip
      )
    );
    UPstream::msgType() = oldTag;
    return interRegionAMI_[nbrRegionID][regionPatchI];
  }
}
mousse::label mousse::regionModels::regionModel::nbrCoupledPatchID
(
  const regionModel& nbrRegion,
  const label regionPatchI
) const
{
  label nbrPatchI = -1;
  // region
  const fvMesh& nbrRegionMesh = nbrRegion.regionMesh();
  // boundary mesh
  const polyBoundaryMesh& nbrPbm = nbrRegionMesh.boundaryMesh();
  const polyBoundaryMesh& pbm = regionMesh().boundaryMesh();
  if (regionPatchI > pbm.size() - 1)
  {
    FatalErrorIn
    (
      "mousse::label mousse::regionModels::regionModel::nbrCoupledPatchID"
      "("
        "const regionModel&, "
        "const label"
      ") const"
    )
      << "region patch index out of bounds: "
      << "region patch index = " << regionPatchI
      << ", maximum index = " << pbm.size() - 1
      << abort(FatalError);
  }
  const polyPatch& pp = regionMesh().boundaryMesh()[regionPatchI];
  if (!isA<mappedPatchBase>(pp))
  {
    FatalErrorIn
    (
      "mousse::label mousse::regionModels::regionModel::nbrCoupledPatchID"
      "("
        "const regionModel&, "
        "const label"
      ") const"
    )
      << "Expected a " << mappedPatchBase::typeName
      << " patch, but found a " << pp.type() << abort(FatalError);
  }
  const mappedPatchBase& mpb = refCast<const mappedPatchBase>(pp);
  // sample patch name on the primary region
  const word& primaryPatchName = mpb.samplePatch();
  // find patch on nbr region that has the same sample patch name
  forAll(nbrRegion.intCoupledPatchIDs(), j)
  {
    const label nbrRegionPatchI = nbrRegion.intCoupledPatchIDs()[j];
    const mappedPatchBase& mpb =
      refCast<const mappedPatchBase>(nbrPbm[nbrRegionPatchI]);
    if (mpb.samplePatch() == primaryPatchName)
    {
      nbrPatchI = nbrRegionPatchI;
      break;
    }
  }
  if (nbrPatchI == -1)
  {
    const polyPatch& p = regionMesh().boundaryMesh()[regionPatchI];
    FatalErrorIn
    (
      "mousse::label mousse::regionModels::regionModel::nbrCoupledPatchID"
      "("
        "const regionModel&, "
        "const label"
      ") const"
    )
      << "Unable to find patch pair for local patch "
      << p.name() << " and region " << nbrRegion.name()
      << abort(FatalError);
  }
  return nbrPatchI;
}
// Constructors 
mousse::regionModels::regionModel::regionModel
(
  const fvMesh& mesh,
  const word& regionType
)
:
  IOdictionary
  (
    IOobject
    (
      regionType + "Properties",
      mesh.time().constant(),
      mesh.time(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    )
  ),
  primaryMesh_(mesh),
  time_(mesh.time()),
  active_(false),
  infoOutput_(false),
  modelName_("none"),
  regionMeshPtr_(NULL),
  coeffs_(dictionary::null),
  outputPropertiesPtr_(NULL),
  primaryPatchIDs_(),
  intCoupledPatchIDs_(),
  regionName_("none"),
  functions_(*this),
  interRegionAMINames_(),
  interRegionAMI_()
{}
mousse::regionModels::regionModel::regionModel
(
  const fvMesh& mesh,
  const word& regionType,
  const word& modelName,
  bool readFields
)
:
  IOdictionary
  (
    IOobject
    (
      regionType + "Properties",
      mesh.time().constant(),
      mesh.time(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    )
  ),
  primaryMesh_(mesh),
  time_(mesh.time()),
  active_(lookup("active")),
  infoOutput_(true),
  modelName_(modelName),
  regionMeshPtr_(NULL),
  coeffs_(subOrEmptyDict(modelName + "Coeffs")),
  outputPropertiesPtr_(NULL),
  primaryPatchIDs_(),
  intCoupledPatchIDs_(),
  regionName_(lookup("regionName")),
  functions_(*this, subOrEmptyDict("functions"))
{
  if (active_)
  {
    constructMeshObjects();
    initialise();
    if (readFields)
    {
      read();
    }
  }
}
mousse::regionModels::regionModel::regionModel
(
  const fvMesh& mesh,
  const word& regionType,
  const word& modelName,
  const dictionary& dict,
  bool readFields
)
:
  IOdictionary
  (
    IOobject
    (
      regionType + "Properties",
      mesh.time().constant(),
      mesh.time(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      true
    ),
    dict
  ),
  primaryMesh_(mesh),
  time_(mesh.time()),
  active_(dict.lookup("active")),
  infoOutput_(false),
  modelName_(modelName),
  regionMeshPtr_(NULL),
  coeffs_(dict.subOrEmptyDict(modelName + "Coeffs")),
  outputPropertiesPtr_(NULL),
  primaryPatchIDs_(),
  intCoupledPatchIDs_(),
  regionName_(dict.lookup("regionName")),
  functions_(*this, subOrEmptyDict("functions"))
{
  if (active_)
  {
    constructMeshObjects(dict);
    initialise();
    if (readFields)
    {
      read(dict);
    }
  }
}
// Destructor 
mousse::regionModels::regionModel::~regionModel()
{}
// Member Functions 
void mousse::regionModels::regionModel::evolve()
{
  if (active_)
  {
    Info<< "\nEvolving " << modelName_ << " for region "
      << regionMesh().name() << endl;
    //read();
    preEvolveRegion();
    evolveRegion();
    postEvolveRegion();
    // Provide some feedback
    if (infoOutput_)
    {
      Info<< incrIndent;
      info();
      Info<< endl << decrIndent;
    }
    if (time_.outputTime())
    {
      outputProperties().writeObject
      (
        IOstream::ASCII,
        IOstream::currentVersion,
        time_.writeCompression()
      );
    }
  }
}
void mousse::regionModels::regionModel::preEvolveRegion()
{
  functions_.preEvolveRegion();
}
void mousse::regionModels::regionModel::evolveRegion()
{
  // do nothing
}
void mousse::regionModels::regionModel::postEvolveRegion()
{
  functions_.postEvolveRegion();
}
void mousse::regionModels::regionModel::info()
{
  // do nothing
}
