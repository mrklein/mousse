// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "filtered_linear.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme
  (
    filteredLinear,
    filteredLinearLimiter
  )
}
