// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_scheme.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_linear.hpp"


namespace mousse {

MAKE_LIMITED_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME
(
  limitedLinear,
  limitedLinearLimiter
)

MAKE_L_LIMITED_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME
(
  limitedLimitedLinear,
  LimitedLimiter,
  limitedLinearLimiter,
  NVDTVD,
  magSqr
)

MAKE_L_LIMITED_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME
(
  limitedLinear01,
  Limited01Limiter,
  limitedLinearLimiter,
  NVDTVD,
  magSqr
)

}

