// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correct_phi.hpp"
#include "surface_fields.hpp"
#include "vol_fields.hpp"
#include "fv_mesh.hpp"


void mousse::correctUphiBCs
(
  volVectorField& U,
  surfaceScalarField& phi
)
{
  const fvMesh& mesh = U.mesh();
  if (mesh.changing()) {
    FOR_ALL(U.boundaryField(), patchi) {
      if (U.boundaryField()[patchi].fixesValue())
      {
        U.boundaryField()[patchi].initEvaluate();
      }
    }
    FOR_ALL(U.boundaryField(), patchi) {
      if (U.boundaryField()[patchi].fixesValue()) {
        U.boundaryField()[patchi].evaluate();
        phi.boundaryField()[patchi] =
          U.boundaryField()[patchi] & mesh.Sf().boundaryField()[patchi];
      }
    }
  }
}


void mousse::correctUphiBCs
(
  const volScalarField& rho,
  volVectorField& U,
  surfaceScalarField& phi
)
{
  const fvMesh& mesh = U.mesh();
  if (mesh.changing()) {
    FOR_ALL(U.boundaryField(), patchi) {
      if (U.boundaryField()[patchi].fixesValue()) {
        U.boundaryField()[patchi].initEvaluate();
      }
    }
    FOR_ALL(U.boundaryField(), patchi) {
      if (U.boundaryField()[patchi].fixesValue()) {
        U.boundaryField()[patchi].evaluate();
        phi.boundaryField()[patchi] =
          rho.boundaryField()[patchi]*(U.boundaryField()[patchi]
                                       & mesh.Sf().boundaryField()[patchi]);
      }
    }
  }
}

