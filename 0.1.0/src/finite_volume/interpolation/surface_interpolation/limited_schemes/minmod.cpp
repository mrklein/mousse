// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "minmod.hpp"


namespace mousse {

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(Minmod, MinmodLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(MinmodV, MinmodLimiter)

}
