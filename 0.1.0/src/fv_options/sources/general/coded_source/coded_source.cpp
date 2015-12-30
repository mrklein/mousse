// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_fv_option.hpp"
#include "_coded_source.hpp"
makeFvOption(CodedSource, scalar);
makeFvOption(CodedSource, vector);
makeFvOption(CodedSource, sphericalTensor);
makeFvOption(CodedSource, symmTensor);
makeFvOption(CodedSource, tensor);
