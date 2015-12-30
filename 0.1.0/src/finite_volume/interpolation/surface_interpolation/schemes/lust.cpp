// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lust.hpp"
namespace mousse
{
  //makeSurfaceInterpolationScheme(LUST);
  makeSurfaceInterpolationTypeScheme(LUST, scalar);
  makeSurfaceInterpolationTypeScheme(LUST, vector);
}
