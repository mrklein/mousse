// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "limited_scheme.hpp"
#include "van_albada.hpp"
namespace mousse
{
  makeLimitedSurfaceInterpolationScheme(vanAlbada, vanAlbadaLimiter)
  makeLimitedVSurfaceInterpolationScheme(vanAlbadaV, vanAlbadaLimiter)
}
