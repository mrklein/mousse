// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_coded_source.hpp"


MAKE_FV_OPTION(CodedSource, scalar);
MAKE_FV_OPTION(CodedSource, vector);
MAKE_FV_OPTION(CodedSource, sphericalTensor);
MAKE_FV_OPTION(CodedSource, symmTensor);
MAKE_FV_OPTION(CodedSource, tensor);

