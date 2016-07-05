#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_REMOVE_REGISTERED_OBJECT_REMOVE_REGISTERED_OBJECT_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_REMOVE_REGISTERED_OBJECT_REMOVE_REGISTERED_OBJECT_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "remove_registered_object.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<removeRegisteredObject>
  removeRegisteredObjectFunctionObject;

}

#endif

