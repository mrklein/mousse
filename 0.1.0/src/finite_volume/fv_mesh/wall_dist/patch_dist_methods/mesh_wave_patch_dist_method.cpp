// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_wave_patch_dist_method.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "patch_wave.hpp"
#include "patch_data_wave.hpp"
#include "wall_point_data.hpp"
#include "empty_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace patchDistMethods
{
  defineTypeNameAndDebug(meshWave, 0);
  addToRunTimeSelectionTable(patchDistMethod, meshWave, dictionary);
}
}
// Constructors 
mousse::patchDistMethods::meshWave::meshWave
(
  const dictionary& dict,
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
:
  patchDistMethod(mesh, patchIDs),
  correctWalls_(dict.lookupOrDefault<Switch>("correctWalls", true)),
  nUnset_(0)
{}
mousse::patchDistMethods::meshWave::meshWave
(
  const fvMesh& mesh,
  const labelHashSet& patchIDs,
  const bool correctWalls
)
:
  patchDistMethod(mesh, patchIDs),
  correctWalls_(correctWalls),
  nUnset_(0)
{}
// Member Functions 
bool mousse::patchDistMethods::meshWave::correct(volScalarField& y)
{
  y = dimensionedScalar("yWall", dimLength, GREAT);
  // Calculate distance starting from patch faces
  patchWave wave(mesh_, patchIDs_, correctWalls_);
  // Transfer cell values from wave into y
  y.transfer(wave.distance());
  // Transfer values on patches into boundaryField of y
  forAll(y.boundaryField(), patchI)
  {
    if (!isA<emptyFvPatchScalarField>(y.boundaryField()[patchI]))
    {
      scalarField& waveFld = wave.patchDistance()[patchI];
      y.boundaryField()[patchI].transfer(waveFld);
    }
  }
  // Transfer number of unset values
  nUnset_ = wave.nUnset();
  return nUnset_ > 0;
}
bool mousse::patchDistMethods::meshWave::correct
(
  volScalarField& y,
  volVectorField& n
)
{
  y = dimensionedScalar("yWall", dimLength, GREAT);
  // Collect pointers to data on patches
  UPtrList<vectorField> patchData(mesh_.boundaryMesh().size());
  forAll(n.boundaryField(), patchI)
  {
    patchData.set(patchI, &n.boundaryField()[patchI]);
  }
  // Do mesh wave
  patchDataWave<wallPointData<vector> > wave
  (
    mesh_,
    patchIDs_,
    patchData,
    correctWalls_
  );
  // Transfer cell values from wave into y and n
  y.transfer(wave.distance());
  n.transfer(wave.cellData());
  // Transfer values on patches into boundaryField of y and n
  forAll(y.boundaryField(), patchI)
  {
    scalarField& waveFld = wave.patchDistance()[patchI];
    if (!isA<emptyFvPatchScalarField>(y.boundaryField()[patchI]))
    {
      y.boundaryField()[patchI].transfer(waveFld);
      vectorField& wavePatchData = wave.patchData()[patchI];
      n.boundaryField()[patchI].transfer(wavePatchData);
    }
  }
  // Transfer number of unset values
  nUnset_ = wave.nUnset();
  return nUnset_ > 0;
}
