// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_explicit_set_value.hpp"
makeFvOption(ExplicitSetValue, scalar);
makeFvOption(ExplicitSetValue, vector);
makeFvOption(ExplicitSetValue, sphericalTensor);
makeFvOption(ExplicitSetValue, symmTensor);
makeFvOption(ExplicitSetValue, tensor);
