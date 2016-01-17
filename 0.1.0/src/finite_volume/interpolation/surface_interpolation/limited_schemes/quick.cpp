// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "quick.hpp"
#include "quickv.hpp"

namespace mousse
{

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(QUICK, QUICKLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(QUICKV, QUICKVLimiter)

}
