// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "super_bee.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(SuperBee, SuperBeeLimiter)
  makeLimitedVSurfaceInterpolationScheme(SuperBeeV, SuperBeeLimiter)
}
