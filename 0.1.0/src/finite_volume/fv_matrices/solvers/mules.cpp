// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mules.hpp"
void mousse::MULES::explicitSolve
(
  volScalarField& psi,
  const surfaceScalarField& phi,
  surfaceScalarField& phiPsi,
  const scalar psiMax,
  const scalar psiMin
)
{
  explicitSolve
  (
    geometricOneField(),
    psi,
    phi,
    phiPsi,
    zeroField(), zeroField(),
    psiMax, psiMin
  );
}
void mousse::MULES::limitSum(UPtrList<scalarField>& phiPsiCorrs)
{
  FOR_ALL(phiPsiCorrs[0], facei)
  {
    scalar sumPos = 0;
    scalar sumNeg = 0;
    for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
    {
      if (phiPsiCorrs[phasei][facei] > 0)
      {
        sumPos += phiPsiCorrs[phasei][facei];
      }
      else
      {
        sumNeg += phiPsiCorrs[phasei][facei];
      }
    }
    scalar sum = sumPos + sumNeg;
    if (sum > 0 && sumPos > VSMALL)
    {
      scalar lambda = -sumNeg/sumPos;
      for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
      {
        if (phiPsiCorrs[phasei][facei] > 0)
        {
          phiPsiCorrs[phasei][facei] *= lambda;
        }
      }
    }
    else if (sum < 0 && sumNeg < -VSMALL)
    {
      scalar lambda = -sumPos/sumNeg;
      for (int phasei=0; phasei<phiPsiCorrs.size(); phasei++)
      {
        if (phiPsiCorrs[phasei][facei] < 0)
        {
          phiPsiCorrs[phasei][facei] *= lambda;
        }
      }
    }
  }
}
