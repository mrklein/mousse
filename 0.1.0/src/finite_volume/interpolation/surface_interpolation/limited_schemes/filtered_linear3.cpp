// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "filtered_linear3.hpp"
#include "filtered_linear3_v.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme
  (
    filteredLinear3,
    filteredLinear3Limiter
  )
  makeLimitedVSurfaceInterpolationScheme
  (
    filteredLinear3V,
    filteredLinear3VLimiter
  )
}
