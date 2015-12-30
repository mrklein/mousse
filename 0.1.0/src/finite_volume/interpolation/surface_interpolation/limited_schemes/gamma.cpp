// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "gamma.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(Gamma, GammaLimiter)
  makeLimitedVSurfaceInterpolationScheme(GammaV, GammaLimiter)
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedGamma,
    LimitedLimiter,
    GammaLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    Gamma01,
    Limited01Limiter,
    GammaLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
}
