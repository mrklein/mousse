// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "muscl.hpp"


namespace mousse {

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(MUSCL, MUSCLLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(MUSCLV, MUSCLLimiter)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedMUSCL,
  LimitedLimiter,
  MUSCLLimiter,
  NVDTVD,
  magSqr,
  scalar
)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  MUSCL01,
  Limited01Limiter,
  MUSCLLimiter,
  NVDTVD,
  magSqr,
  scalar
)

}
