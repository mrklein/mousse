#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_RESIDUALS_RESIDUALS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_RESIDUALS_RESIDUALS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "residuals.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<residuals>
  residualsFunctionObject;

}

#endif

