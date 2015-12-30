// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline mousse::scalar mousse::basicCombustionMixture::fres
(
  const scalar ft,
  const scalar stoicRatio
) const
{
  return max(ft - (scalar(1) - ft)/stoicRatio, scalar(0));
}
inline mousse::tmp<mousse::volScalarField> mousse::basicCombustionMixture::fres
(
  const volScalarField& ft,
  const dimensionedScalar& stoicRatio
) const
{
  return max(ft - (scalar(1) - ft)/stoicRatio.value(), scalar(0));
}
