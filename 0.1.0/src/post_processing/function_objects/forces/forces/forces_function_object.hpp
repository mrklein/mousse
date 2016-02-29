#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FORCES_FORCES_FORCES_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FORCES_FORCES_FORCES_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   forces_function_object.cpp
#include "forces.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<forces> forcesFunctionObject;
}
#endif
