// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poisson_patch_dist_method.hpp"
#include "fvc_grad.hpp"
#include "fvm_laplacian.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"

// Static Data Members
namespace mousse
{
namespace patchDistMethods
{
  DEFINE_TYPE_NAME_AND_DEBUG(Poisson, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(patchDistMethod, Poisson, dictionary);
}
}

// Constructors
mousse::patchDistMethods::Poisson::Poisson
(
  const dictionary&,
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
:
  patchDistMethod{mesh, patchIDs}
{}
mousse::patchDistMethods::Poisson::Poisson
(
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
:
  patchDistMethod(mesh, patchIDs)
{}

// Member Functions
bool mousse::patchDistMethods::Poisson::correct(volScalarField& y)
{
  return correct(y, const_cast<volVectorField&>(volVectorField::null()));
}
bool mousse::patchDistMethods::Poisson::correct
(
  volScalarField& y,
  volVectorField& n
)
{
  if (!tyPsi_.valid())
  {
    tyPsi_ = tmp<volScalarField>
    (
      new volScalarField
      (
        IOobject
        (
          "yPsi",
          mesh_.time().timeName(),
          mesh_
        ),
        mesh_,
        dimensionedScalar("yPsi", sqr(dimLength), 0.0),
        y.boundaryField().types()
      )
    );
  }
  volScalarField& yPsi = tyPsi_();
  solve(fvm::laplacian(yPsi) == dimensionedScalar("1", dimless, -1.0));
  volVectorField gradyPsi(fvc::grad(yPsi));
  volScalarField magGradyPsi(mag(gradyPsi));
  y = sqrt(magSqr(gradyPsi) + 2*yPsi) - magGradyPsi;
  // Cache yPsi if the mesh is moving otherwise delete
  if (!mesh_.changing())
  {
    tyPsi_.clear();
  }
  // Only calculate n if the field is defined
  if (notNull(n))
  {
    n =
     -gradyPsi
     /max
      (
        magGradyPsi,
        dimensionedScalar("smallMagGradyPsi", dimLength, SMALL)
      );
  }
  return true;
}
