// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inverse_volume_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "patch_wave.hpp"
#include "hash_set.hpp"
#include "surface_interpolate.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(inverseVolumeDiffusivity, 0);
  addToRunTimeSelectionTable
  (
    motionDiffusivity,
    inverseVolumeDiffusivity,
    Istream
  );
}
// Constructors 
mousse::inverseVolumeDiffusivity::inverseVolumeDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity(mesh, mdData)
{
  correct();
}
// Destructor 
mousse::inverseVolumeDiffusivity::~inverseVolumeDiffusivity()
{}
// Member Functions 
void mousse::inverseVolumeDiffusivity::correct()
{
  volScalarField V
  (
    IOobject
    (
      "V",
      mesh().time().timeName(),
      mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh(),
    dimless,
    zeroGradientFvPatchScalarField::typeName
  );
  V.internalField() = mesh().V();
  V.correctBoundaryConditions();
  faceDiffusivity_ = 1.0/fvc::interpolate(V);
}
