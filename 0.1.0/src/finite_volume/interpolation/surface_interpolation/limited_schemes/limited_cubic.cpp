// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "limited_cubic.hpp"
#include "limited_cubic_v.hpp"


namespace mousse {

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(limitedCubic, limitedCubicLimiter)

MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME
(
  limitedCubicV,
  limitedCubicVLimiter
)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedLimitedCubic,
  LimitedLimiter,
  limitedCubicLimiter,
  NVDTVD,
  magSqr,
  scalar
)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedCubic01,
  Limited01Limiter,
  limitedCubicLimiter,
  NVDTVD,
  magSqr,
  scalar
)

}

