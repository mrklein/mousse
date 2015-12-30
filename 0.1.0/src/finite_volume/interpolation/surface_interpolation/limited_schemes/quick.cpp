// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "quick.hpp"
#include "quickv.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(QUICK, QUICKLimiter)
  makeLimitedVSurfaceInterpolationScheme(QUICKV, QUICKVLimiter)
}
