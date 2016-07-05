#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_BLENDING_FACTOR_BLENDING_FACTOR_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_BLENDING_FACTOR_BLENDING_FACTOR_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_factor.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<blendingFactor>
  blendingFactorFunctionObject;

}

#endif

