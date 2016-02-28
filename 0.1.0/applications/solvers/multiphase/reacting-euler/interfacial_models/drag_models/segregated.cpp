// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "segregated.hpp"
#include "phase_pair.hpp"
#include "fvc_grad.hpp"
#include "surface_interpolate.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(segregated, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(dragModel, segregated, dictionary);
}
}
// Constructors 
mousse::dragModels::segregated::segregated
(
  const dictionary& dict,
  const phasePair& pair,
  const bool registerObject
)
:
  dragModel{dict, pair, registerObject},
  m_{"m", dimless, dict},
  n_{"n", dimless, dict}
{}
// Destructor 
mousse::dragModels::segregated::~segregated()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::segregated::CdRe() const
{
  FATAL_ERROR_IN("mousse::dragModels::segregated::CdRe() const")
    << "Not implemented."
    << "Drag coefficient not defined for the segregated model."
    << exit(FatalError);
  return pair_.phase1();
}
mousse::tmp<mousse::volScalarField> mousse::dragModels::segregated::K() const
{
  const fvMesh& mesh = pair_.phase1().mesh();
  const volScalarField& alpha1 = pair_.phase1();
  const volScalarField& alpha2 = pair_.phase2();
  const volScalarField& rho1 = pair_.phase1().rho();
  const volScalarField& rho2 = pair_.phase2().rho();
  tmp<volScalarField> tnu1(pair_.phase1().nu());
  tmp<volScalarField> tnu2(pair_.phase2().nu());
  const volScalarField& nu1 = tnu1();
  const volScalarField& nu2 = tnu2();
  volScalarField L
  {
    {
      "L",
      mesh.time().timeName(),
      mesh
    },
    mesh,
    {"L", dimLength, 0},
    zeroGradientFvPatchField<scalar>::typeName
  };
  L.internalField() = cbrt(mesh.V());
  L.correctBoundaryConditions();
  volScalarField I
  {
    alpha1/max
    (
      alpha1 + alpha2,
      pair_.phase1().residualAlpha() + pair_.phase2().residualAlpha()
    )
  };
  volScalarField magGradI
  {
    max
    (
      mag(fvc::grad(I)),
      (pair_.phase1().residualAlpha() + pair_.phase2().residualAlpha())/L
    )
  };
  volScalarField muI{rho1*nu1*rho2*nu2/(rho1*nu1 + rho2*nu2)};
  volScalarField muAlphaI
  {
    alpha1*rho1*nu1*alpha2*rho2*nu2
    /(max(alpha1, pair_.phase1().residualAlpha())*rho1*nu1
      + max(alpha2, pair_.phase2().residualAlpha())*rho2*nu2)
  };
  volScalarField ReI
  {
    pair_.rho()*pair_.magUr()/(magGradI*muI)
  };
  volScalarField lambda{m_*ReI + n_*muAlphaI/muI};
  return lambda*sqr(magGradI)*muI;
}
mousse::tmp<mousse::surfaceScalarField> mousse::dragModels::segregated::Kf() const
{
  return fvc::interpolate(K());
}
