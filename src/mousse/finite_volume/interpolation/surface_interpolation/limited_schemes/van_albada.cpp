// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_fields.hpp"
#include "limited_scheme.hpp"
#include "van_albada.hpp"


namespace mousse {

MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(vanAlbada, vanAlbadaLimiter)
MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(vanAlbadaV, vanAlbadaLimiter)

}

