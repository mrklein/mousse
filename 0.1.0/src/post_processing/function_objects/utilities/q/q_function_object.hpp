#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_Q_Q_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_Q_Q_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   q_function_object.cpp
#include "q.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<Q> QFunctionObject;
}
#endif
