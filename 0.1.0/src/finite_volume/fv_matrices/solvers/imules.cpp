// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "imules.hpp"


void mousse::MULES::implicitSolve
(
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsi,
  const scalar psiMax,
  const scalar psiMin
)
{
  implicitSolve
  (
    geometricOneField(),
    psi,
    phi,
    phiPsi,
    zeroField(), zeroField(),
    psiMax, psiMin
  );
}

