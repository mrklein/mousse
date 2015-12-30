// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "muscl.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(MUSCL, MUSCLLimiter)
  makeLimitedVSurfaceInterpolationScheme(MUSCLV, MUSCLLimiter)
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    limitedMUSCL,
    LimitedLimiter,
    MUSCLLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
  makeLLimitedSurfaceInterpolationTypeScheme
  (
    MUSCL01,
    Limited01Limiter,
    MUSCLLimiter,
    NVDTVD,
    magSqr,
    scalar
  )
/*
  makeLimitedSurfaceInterpolationTypeScheme
  (
    MUSCL,
    MUSCLLimiter,
    NVDTVD,
    rhoMagSqr,
    scalar
  )
  makeLimitedSurfaceInterpolationTypeScheme
  (
    MUSCL,
    MUSCLLimiter,
    NVDTVD,
    rhoMagSqr,
    vector
  )
*/
}
