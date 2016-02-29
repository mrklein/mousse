#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_REGISTERED_OBJECT_WRITE_REGISTERED_OBJECT_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_REGISTERED_OBJECT_WRITE_REGISTERED_OBJECT_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_registered_object_function_object.cpp
#include "write_registered_object.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<writeRegisteredObject>
    writeRegisteredObjectFunctionObject;
}
#endif
