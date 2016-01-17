// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "van_leer.hpp"

namespace mousse
{

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(vanLeer, vanLeerLimiter)

MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(vanLeerV, vanLeerLimiter)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  limitedVanLeer,
  LimitedLimiter,
  vanLeerLimiter,
  NVDTVD,
  magSqr,
  scalar
)

MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME
(
  vanLeer01,
  Limited01Limiter,
  vanLeerLimiter,
  NVDTVD,
  magSqr,
  scalar
)

}
