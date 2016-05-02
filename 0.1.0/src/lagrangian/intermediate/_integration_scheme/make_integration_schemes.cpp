// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_integration_scheme.hpp"
#include "_euler.hpp"
#include "_analytical.hpp"
#include "scalar.hpp"
#include "vector.hpp"


namespace mousse {

MAKE_INTEGRATION_SCHEME(scalar);
MAKE_INTEGRATION_SCHEME_TYPE(Euler, scalar);
MAKE_INTEGRATION_SCHEME_TYPE(Analytical, scalar);
MAKE_INTEGRATION_SCHEME(vector);
MAKE_INTEGRATION_SCHEME_TYPE(Euler, vector);
MAKE_INTEGRATION_SCHEME_TYPE(Analytical, vector);

}

