// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "minmod.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(Minmod, MinmodLimiter)
  makeLimitedVSurfaceInterpolationScheme(MinmodV, MinmodLimiter)
}
