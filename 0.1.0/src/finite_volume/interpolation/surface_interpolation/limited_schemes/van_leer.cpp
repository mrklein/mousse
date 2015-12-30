// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "van_leer.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(vanLeer, vanLeerLimiter)
  makeLimitedVSurfaceInterpolationScheme(vanLeerV, vanLeerLimiter)
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedVanLeer,
    LimitedLimiter,
    vanLeerLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    vanLeer01,
    Limited01Limiter,
    vanLeerLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
}
