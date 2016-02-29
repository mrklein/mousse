#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PECLET_PECLET_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PECLET_PECLET_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   peclet_function_object.cpp
#include "peclet.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<Peclet> PecletFunctionObject;
}
#endif
