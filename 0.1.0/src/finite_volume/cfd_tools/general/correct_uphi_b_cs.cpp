// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correct_phi.hpp"
void mousse::correctUphiBCs
(
  volVectorField& U,
  surfaceScalarField& phi
)
{
  const fvMesh& mesh = U.mesh();
  if (mesh.changing())
  {
    forAll(U.boundaryField(), patchi)
    {
      if (U.boundaryField()[patchi].fixesValue())
      {
        U.boundaryField()[patchi].initEvaluate();
      }
    }
    forAll(U.boundaryField(), patchi)
    {
      if (U.boundaryField()[patchi].fixesValue())
      {
        U.boundaryField()[patchi].evaluate();
        phi.boundaryField()[patchi] =
          U.boundaryField()[patchi]
         & mesh.Sf().boundaryField()[patchi];
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
  if (mesh.changing())
  {
    forAll(U.boundaryField(), patchi)
    {
      if (U.boundaryField()[patchi].fixesValue())
      {
        U.boundaryField()[patchi].initEvaluate();
      }
    }
    forAll(U.boundaryField(), patchi)
    {
      if (U.boundaryField()[patchi].fixesValue())
      {
        U.boundaryField()[patchi].evaluate();
        phi.boundaryField()[patchi] =
          rho.boundaryField()[patchi]
         *(
            U.boundaryField()[patchi]
           & mesh.Sf().boundaryField()[patchi]
          );
      }
    }
  }
}
