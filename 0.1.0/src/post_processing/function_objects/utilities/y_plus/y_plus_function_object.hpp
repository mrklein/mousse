#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_Y_PLUS_Y_PLUS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_Y_PLUS_Y_PLUS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   y_plus_function_object.cpp
#include "y_plus.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<yPlus> yPlusFunctionObject;
}
#endif
