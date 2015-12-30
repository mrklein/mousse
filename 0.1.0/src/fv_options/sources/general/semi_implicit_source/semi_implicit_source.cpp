// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_semi_implicit_source.hpp"
makeFvOption(SemiImplicitSource, scalar);
makeFvOption(SemiImplicitSource, vector);
makeFvOption(SemiImplicitSource, sphericalTensor);
makeFvOption(SemiImplicitSource, symmTensor);
makeFvOption(SemiImplicitSource, tensor);
