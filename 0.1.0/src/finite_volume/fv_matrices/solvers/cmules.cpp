// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cmules.hpp"
void mousse::MULES::correct
(
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsiCorr,
  const scalar psiMax,
  const scalar psiMin
)
{
  correct
  (
    geometricOneField(),
    psi,
    phi,
    phiPsiCorr,
    zeroField(), zeroField(),
    psiMax, psiMin
  );
}
