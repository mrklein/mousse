// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_dist_data.hpp"
#include "patch_data_wave.hpp"
#include "wall_poly_patch.hpp"
#include "empty_fv_patch_fields.hpp"
// Constructors 
// Construct from components
template<class TransferType>
mousse::wallDistData<TransferType>::wallDistData
(
  const mousse::fvMesh& mesh,
  GeometricField<Type, fvPatchField, volMesh>& field,
  const bool correctWalls
)
:
  volScalarField
  (
    IOobject
    (
      "y",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimensionedScalar("y", dimLength, GREAT)
  ),
  cellDistFuncs(mesh),
  field_(field),
  correctWalls_(correctWalls),
  nUnset_(0)
{
  correct();
}
// Destructor 
template<class TransferType>
mousse::wallDistData<TransferType>::~wallDistData()
{}
// Member Functions 
// Correct for mesh geom/topo changes
template<class TransferType>
void mousse::wallDistData<TransferType>::correct()
{
  const polyMesh& mesh = cellDistFuncs::mesh();
  //
  // Fill data on wall patches with initial values
  //
  // Get patchids of walls
  labelHashSet wallPatchIDs(getPatchIDs<wallPolyPatch>());
  // Collect pointers to data on patches
  UPtrList<Field<Type> > patchData(mesh.boundaryMesh().size());
  FOR_ALL(field_.boundaryField(), patchI)
  {
    patchData.set(patchI, &field_.boundaryField()[patchI]);
  }
  // Do mesh wave
  patchDataWave<TransferType> wave
  (
    mesh,
    wallPatchIDs,
    patchData,
    correctWalls_
  );
  // Transfer cell values from wave into *this and field_
  transfer(wave.distance());
  field_.transfer(wave.cellData());
  // Transfer values on patches into boundaryField of *this and field_
  FOR_ALL(boundaryField(), patchI)
  {
    scalarField& waveFld = wave.patchDistance()[patchI];
    if (!isA<emptyFvPatchScalarField>(boundaryField()[patchI]))
    {
      boundaryField()[patchI].transfer(waveFld);
      Field<Type>& wavePatchData = wave.patchData()[patchI];
      field_.boundaryField()[patchI].transfer(wavePatchData);
    }
  }
  // Transfer number of unset values
  nUnset_ = wave.nUnset();
}
