// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inverse_distance_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "patch_wave.hpp"
#include "hash_set.hpp"
#include "surface_interpolate.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(inverseDistanceDiffusivity, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    motionDiffusivity,
    inverseDistanceDiffusivity,
    Istream
  );
}
// Constructors 
mousse::inverseDistanceDiffusivity::inverseDistanceDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity(mesh, mdData),
  patchNames_(mdData)
{
  correct();
}
// Destructor 
mousse::inverseDistanceDiffusivity::~inverseDistanceDiffusivity()
{}
// Member Functions 
mousse::tmp<mousse::scalarField> mousse::inverseDistanceDiffusivity::y() const
{
  labelHashSet patchSet(mesh().boundaryMesh().patchSet(patchNames_));
  if (patchSet.size())
  {
    return tmp<scalarField>
    (
      new scalarField(patchWave(mesh(), patchSet, false).distance())
    );
  }
  else
  {
    return tmp<scalarField>(new scalarField(mesh().nCells(), 1.0));
  }
}
void mousse::inverseDistanceDiffusivity::correct()
{
  volScalarField y_
  (
    IOobject
    (
      "y",
      mesh().time().timeName(),
      mesh()
    ),
    mesh(),
    dimless,
    zeroGradientFvPatchScalarField::typeName
  );
  y_.internalField() = y();
  y_.correctBoundaryConditions();
  faceDiffusivity_ = 1.0/fvc::interpolate(y_);
}
