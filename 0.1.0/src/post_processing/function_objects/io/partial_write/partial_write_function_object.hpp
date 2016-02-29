#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_PARTIAL_WRITE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_PARTIAL_WRITE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   partial_write_function_object.cpp
#include "partial_write.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<partialWrite>
    partialWriteFunctionObject;
}
#endif
