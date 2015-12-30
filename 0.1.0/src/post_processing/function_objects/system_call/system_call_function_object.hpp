// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   system_call_function_object.cpp
#ifndef system_call_function_object_hpp_
#define system_call_function_object_hpp_
#include "system_call.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<systemCall>
    systemCallFunctionObject;
}
#endif
