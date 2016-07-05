#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_GRAD_CALC_FVC_GRAD_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_GRAD_CALC_FVC_GRAD_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_fvc_grad.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<calcFvcGrad> calcFvcGradFunctionObject;

}

#endif

