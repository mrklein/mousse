// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_scheme.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_linear.hpp"
namespace mousse
{
  makeLimitedMultivariateSurfaceInterpolationScheme
  (
    limitedLinear,
    limitedLinearLimiter
  )
  makeLLimitedMultivariateSurfaceInterpolationScheme
  (
    limitedLimitedLinear,
    LimitedLimiter,
    limitedLinearLimiter,
    NVDTVD,
    magSqr
  )
  makeLLimitedMultivariateSurfaceInterpolationScheme
  (
    limitedLinear01,
    Limited01Limiter,
    limitedLinearLimiter,
    NVDTVD,
    magSqr
  )
}
