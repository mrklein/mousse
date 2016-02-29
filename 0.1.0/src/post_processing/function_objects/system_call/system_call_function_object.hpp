#ifndef POST_PROCESSING_FUNCTION_OBJECTS_SYSTEM_CALL_SYSTEM_CALL_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_SYSTEM_CALL_SYSTEM_CALL_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   system_call_function_object.cpp
#include "system_call.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<systemCall>
    systemCallFunctionObject;
}
#endif
