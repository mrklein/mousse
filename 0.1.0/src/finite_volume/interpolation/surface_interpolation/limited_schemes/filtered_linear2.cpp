// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "filtered_linear2.hpp"
#include "filtered_linear2_v.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme
  (
    filteredLinear2,
    filteredLinear2Limiter
  )
  makeLimitedVSurfaceInterpolationScheme
  (
    filteredLinear2V,
    filteredLinear2VLimiter
  )
}
