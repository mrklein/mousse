// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correct_phi.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_scalar_matrix.hpp"
#include "fvm_ddt.hpp"
#include "fvm_laplacian.hpp"
#include "fvc_div.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "adjust_phi.hpp"
#include "fvc_mesh_phi.hpp"
#include "pimple_control.hpp"


template<class RAUfType, class DivUType>
void mousse::CorrectPhi
(
  volVectorField& U,
  surfaceScalarField& phi,
  const volScalarField& p,
  const RAUfType& rAUf,
  const DivUType& divU,
  pimpleControl& pimple
)
{
  const fvMesh& mesh = U.mesh();
  const Time& runTime = mesh.time();
  correctUphiBCs(U, phi);
  // Initialize BCs list for pcorr to zero-gradient
  wordList pcorrTypes
  {
    p.boundaryField().size(),
    zeroGradientFvPatchScalarField::typeName
  };
  // Set BCs of pcorr to fixed-value for patches at which p is fixed
  FOR_ALL(p.boundaryField(), patchi) {
    if (p.boundaryField()[patchi].fixesValue()) {
      pcorrTypes[patchi] = fixedValueFvPatchScalarField::typeName;
    }
  }
  volScalarField pcorr
  {
    {
      "pcorr",
      runTime.timeName(),
      mesh
    },
    mesh,
    {"pcorr", p.dimensions(), 0.0},
    pcorrTypes
  };
  if (pcorr.needReference()) {
    fvc::makeRelative(phi, U);
    adjustPhi(phi, U, pcorr);
    fvc::makeAbsolute(phi, U);
  }
  mesh.setFluxRequired(pcorr.name());
  while (pimple.correctNonOrthogonal()) {
    // Solve for pcorr such that the divergence of the corrected flux
    // matches the divU provided (from previous iteration, time-step...)
    fvScalarMatrix pcorrEqn
    {
      fvm::laplacian(rAUf, pcorr) == fvc::div(phi) - divU
    };
    pcorrEqn.setReference(0, 0);
    pcorrEqn.solve();
    if (pimple.finalNonOrthogonalIter()) {
      phi -= pcorrEqn.flux();
    }
  }
}


template<class RAUfType, class DivRhoUType>
void mousse::CorrectPhi
(
  volVectorField& U,
  surfaceScalarField& phi,
  const volScalarField& p,
  const volScalarField& rho,
  const volScalarField& psi,
  const RAUfType& rAUf,
  const DivRhoUType& divRhoU,
  pimpleControl& pimple
)
{
  const fvMesh& mesh = U.mesh();
  const Time& runTime = mesh.time();
  correctUphiBCs(rho, U, phi);
  // Initialize BCs list for pcorr to zero-gradient
  wordList pcorrTypes
  {
    p.boundaryField().size(),
    zeroGradientFvPatchScalarField::typeName
  };
  // Set BCs of pcorr to fixed-value for patches at which p is fixed
  FOR_ALL(p.boundaryField(), patchi) {
    if (p.boundaryField()[patchi].fixesValue()) {
      pcorrTypes[patchi] = fixedValueFvPatchScalarField::typeName;
    }
  }
  volScalarField pcorr
  {
    {
      "pcorr",
      runTime.timeName(),
      mesh
    },
    mesh,
    dimensionedScalar("pcorr", p.dimensions(), 0.0),
    pcorrTypes
  };
  mesh.setFluxRequired(pcorr.name());
  while (pimple.correctNonOrthogonal()) {
    // Solve for pcorr such that the divergence of the corrected flux
    // matches the divRhoU provided (from previous iteration, time-step...)
    fvScalarMatrix pcorrEqn
    {
      fvm::ddt(psi, pcorr)
      + fvc::div(phi)
      - fvm::laplacian(rAUf, pcorr)
      ==
      divRhoU
    };
    pcorrEqn.solve();
    if (pimple.finalNonOrthogonalIter()) {
      phi += pcorrEqn.flux();
    }
  }
}

