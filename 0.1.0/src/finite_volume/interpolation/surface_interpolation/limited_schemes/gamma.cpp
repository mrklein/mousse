// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "surface_fields.hpp"
#include "gamma.hpp"

namespace mousse
{

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(Gamma, GammaLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(GammaV, GammaLimiter)
MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedGamma,
  LimitedLimiter,
  GammaLimiter,
  NVDTVD,
  magSqr,
  scalar
)
MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  Gamma01,
  Limited01Limiter,
  GammaLimiter,
  NVDTVD,
  magSqr,
  scalar
)

}
