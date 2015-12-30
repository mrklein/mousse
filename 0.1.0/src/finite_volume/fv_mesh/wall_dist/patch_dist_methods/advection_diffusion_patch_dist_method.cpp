// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "advection_diffusion_patch_dist_method.hpp"
#include "surface_interpolate.hpp"
#include "fvc_grad.hpp"
#include "fvc_div.hpp"
#include "fvm_div.hpp"
#include "fvm_laplacian.hpp"
#include "fvm_sup.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Static Data Members
namespace mousse
{
namespace patchDistMethods
{
  defineTypeNameAndDebug(advectionDiffusion, 0);
  addToRunTimeSelectionTable(patchDistMethod, advectionDiffusion, dictionary);
}
}
// Constructors 
mousse::patchDistMethods::advectionDiffusion::advectionDiffusion
(
  const dictionary& dict,
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
:
  patchDistMethod(mesh, patchIDs),
  coeffs_(dict.subDict(type() + "Coeffs")),
  pdmPredictor_
  (
    patchDistMethod::New
    (
      coeffs_,
      mesh,
      patchIDs
    )
  ),
  epsilon_(coeffs_.lookupOrDefault<scalar>("epsilon", 0.1)),
  tolerance_(coeffs_.lookupOrDefault<scalar>("tolerance", 1e-3)),
  maxIter_(coeffs_.lookupOrDefault<int>("maxIter", 10)),
  predicted_(false)
{}
// Member Functions 
bool mousse::patchDistMethods::advectionDiffusion::correct(volScalarField& y)
{
  return correct(y, const_cast<volVectorField&>(volVectorField::null()));
}
bool mousse::patchDistMethods::advectionDiffusion::correct
(
  volScalarField& y,
  volVectorField& n
)
{
  if (!predicted_)
  {
    pdmPredictor_->correct(y);
    predicted_ = true;
  }
  volVectorField ny
  (
    IOobject
    (
      "ny",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh_,
    dimensionedVector("ny", dimless, vector::zero),
    patchTypes<vector>(mesh_, patchIDs_)
  );
  const fvPatchList& patches = mesh_.boundary();
  forAllConstIter(labelHashSet, patchIDs_, iter)
  {
    label patchi = iter.key();
    ny.boundaryField()[patchi] == -patches[patchi].nf();
  }
  int iter = 0;
  scalar initialResidual = 0;
  do
  {
    ny = fvc::grad(y);
    ny /= (mag(ny) + SMALL);
    surfaceVectorField nf(fvc::interpolate(ny));
    nf /= (mag(nf) + SMALL);
    surfaceScalarField yPhi("yPhi", mesh_.Sf() & nf);
    fvScalarMatrix yEqn
    (
      fvm::div(yPhi, y)
     - fvm::Sp(fvc::div(yPhi), y)
     - epsilon_*y*fvm::laplacian(y)
    ==
      dimensionedScalar("1", dimless, 1.0)
    );
    yEqn.relax();
    initialResidual = yEqn.solve().initialResidual();
  } while (initialResidual > tolerance_ && ++iter < maxIter_);
  // Only calculate n if the field is defined
  if (notNull(n))
  {
    n = -ny;
  }
  return true;
}
