// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_linear.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(limitedLinear, limitedLinearLimiter)
  makeLimitedVSurfaceInterpolationScheme
  (
    limitedLinearV,
    limitedLinearLimiter
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedLimitedLinear,
    LimitedLimiter,
    limitedLinearLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedLinear01,
    Limited01Limiter,
    limitedLinearLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
}
