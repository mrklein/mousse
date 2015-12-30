// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_cubic.hpp"
#include "limited_cubic_v.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(limitedCubic, limitedCubicLimiter)
  makeLimitedVSurfaceInterpolationScheme(limitedCubicV, limitedCubicVLimiter)
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedLimitedCubic,
    LimitedLimiter,
    limitedCubicLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedCubic01,
    Limited01Limiter,
    limitedCubicLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
}
