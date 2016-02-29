#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_COURANT_NO_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_COURANT_NO_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   courant_no_function_object.cpp
#include "courant_no.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<CourantNo> CourantNoFunctionObject;
}
#endif
