// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_explicit_set_value.hpp"


MAKE_FV_OPTION(ExplicitSetValue, scalar);
MAKE_FV_OPTION(ExplicitSetValue, vector);
MAKE_FV_OPTION(ExplicitSetValue, sphericalTensor);
MAKE_FV_OPTION(ExplicitSetValue, symmTensor);
MAKE_FV_OPTION(ExplicitSetValue, tensor);

