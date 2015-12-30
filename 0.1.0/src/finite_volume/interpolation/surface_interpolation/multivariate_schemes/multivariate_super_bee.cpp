// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_scheme.hpp"
#include "limited_scheme.hpp"
#include "limited01.hpp"
#include "super_bee.hpp"
namespace mousse
{
  makeLimitedMultivariateSurfaceInterpolationScheme
  (
    SuperBee,
    SuperBeeLimiter
  )
}
