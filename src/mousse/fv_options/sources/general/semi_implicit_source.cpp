// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_semi_implicit_source.hpp"


MAKE_FV_OPTION(SemiImplicitSource, scalar);
MAKE_FV_OPTION(SemiImplicitSource, vector);
MAKE_FV_OPTION(SemiImplicitSource, sphericalTensor);
MAKE_FV_OPTION(SemiImplicitSource, symmTensor);
MAKE_FV_OPTION(SemiImplicitSource, tensor);

