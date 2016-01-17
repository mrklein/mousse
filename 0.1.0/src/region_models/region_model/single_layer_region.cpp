// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_layer_region.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Static Data Members
namespace mousse
{
namespace regionModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(singleLayerRegion, 0);
}
}
// Private Member Functions 
void mousse::regionModels::singleLayerRegion::constructMeshObjects()
{
  // construct patch normal vectors
  nHatPtr_.reset
  (
    new volVectorField
    (
      IOobject
      (
        "nHat",
        time_.timeName(),
        regionMesh(),
        IOobject::READ_IF_PRESENT,
        NO_WRITE
      ),
      regionMesh(),
      dimensionedVector("zero", dimless, vector::zero),
      zeroGradientFvPatchField<vector>::typeName
    )
  );
  // construct patch areas
  magSfPtr_.reset
  (
    new volScalarField
    (
      IOobject
      (
        "magSf",
        time_.timeName(),
        regionMesh(),
        IOobject::READ_IF_PRESENT,
        NO_WRITE
      ),
      regionMesh(),
      dimensionedScalar("zero", dimArea, 0.0),
      zeroGradientFvPatchField<scalar>::typeName
    )
  );
}
void mousse::regionModels::singleLayerRegion::initialise()
{
  if (debug)
  {
    Pout<< "singleLayerRegion::initialise()" << endl;
  }
  label nBoundaryFaces = 0;
  const polyBoundaryMesh& rbm = regionMesh().boundaryMesh();
  volVectorField& nHat = nHatPtr_();
  volScalarField& magSf = magSfPtr_();
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    const label patchI = intCoupledPatchIDs_[i];
    const polyPatch& pp = rbm[patchI];
    const labelList& fCells = pp.faceCells();
    nBoundaryFaces += fCells.size();
    UIndirectList<vector>(nHat, fCells) = pp.faceNormals();
    UIndirectList<scalar>(magSf, fCells) = mag(pp.faceAreas());
  }
  nHat.correctBoundaryConditions();
  magSf.correctBoundaryConditions();
  if (nBoundaryFaces != regionMesh().nCells())
  {
    FATAL_ERROR_IN("singleLayerRegion::initialise()")
      << "Number of primary region coupled boundary faces not equal to "
      << "the number of cells in the local region" << nl << nl
      << "Number of cells = " << regionMesh().nCells() << nl
      << "Boundary faces  = " << nBoundaryFaces << nl
      << abort(FatalError);
  }
  scalarField passiveMagSf(magSf.size(), 0.0);
  passivePatchIDs_.setSize(intCoupledPatchIDs_.size(), -1);
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    const label patchI = intCoupledPatchIDs_[i];
    const polyPatch& ppIntCoupled = rbm[patchI];
    if (ppIntCoupled.size() > 0)
    {
      label cellId = rbm[patchI].faceCells()[0];
      const cell& cFaces = regionMesh().cells()[cellId];
      label faceI = ppIntCoupled.start();
      label faceO = cFaces.opposingFaceLabel(faceI, regionMesh().faces());
      label passivePatchI = rbm.whichPatch(faceO);
      passivePatchIDs_[i] = passivePatchI;
      const polyPatch& ppPassive = rbm[passivePatchI];
      UIndirectList<scalar>(passiveMagSf, ppPassive.faceCells()) =
        mag(ppPassive.faceAreas());
    }
  }
  Pstream::listCombineGather(passivePatchIDs_, maxEqOp<label>());
  Pstream::listCombineScatter(passivePatchIDs_);
  magSf.field() = 0.5*(magSf + passiveMagSf);
  magSf.correctBoundaryConditions();
}
// Protected Member Functions 
bool mousse::regionModels::singleLayerRegion::read()
{
  return regionModel::read();
}
// Constructors 
mousse::regionModels::singleLayerRegion::singleLayerRegion
(
  const fvMesh& mesh,
  const word& regionType
)
:
  regionModel(mesh, regionType),
  nHatPtr_(NULL),
  magSfPtr_(NULL),
  passivePatchIDs_()
{}
mousse::regionModels::singleLayerRegion::singleLayerRegion
(
  const fvMesh& mesh,
  const word& regionType,
  const word& modelName,
  bool readFields
)
:
  regionModel(mesh, regionType, modelName, false),
  nHatPtr_(NULL),
  magSfPtr_(NULL),
  passivePatchIDs_()
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
// Destructor 
mousse::regionModels::singleLayerRegion::~singleLayerRegion()
{}
// Member Functions 
const mousse::volVectorField& mousse::regionModels::singleLayerRegion::nHat() const
{
  if (!nHatPtr_.valid())
  {
    FATAL_ERROR_IN("const fvMesh& singleLayerRegion::nHat() const")
      << "Region patch normal vectors not available"
      << abort(FatalError);
  }
  return nHatPtr_();
}
const mousse::volScalarField& mousse::regionModels::singleLayerRegion::magSf() const
{
  if (!magSfPtr_.valid())
  {
    FATAL_ERROR_IN("const fvMesh& singleLayerRegion::magSf() const")
      << "Region patch areas not available"
      << abort(FatalError);
  }
  return magSfPtr_();
}
const mousse::labelList&
mousse::regionModels::singleLayerRegion::passivePatchIDs() const
{
  return passivePatchIDs_;
}
