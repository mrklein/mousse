#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PRESSURE_TOOLS_PRESSURE_TOOLS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PRESSURE_TOOLS_PRESSURE_TOOLS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   pressure_tools_function_object.cpp
#include "pressure_tools.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<pressureTools>
    pressureToolsFunctionObject;
}
#endif
