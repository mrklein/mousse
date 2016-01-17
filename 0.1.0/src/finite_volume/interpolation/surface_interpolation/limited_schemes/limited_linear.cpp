// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_linear.hpp"

namespace mousse
{

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(limitedLinear, limitedLinearLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME
(
  limitedLinearV,
  limitedLinearLimiter
)
MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedLimitedLinear,
  LimitedLimiter,
  limitedLinearLimiter,
  NVDTVD,
  magSqr,
  scalar
)
MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedLinear01,
  Limited01Limiter,
  limitedLinearLimiter,
  NVDTVD,
  magSqr,
  scalar
)

}
