// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   partial_write_function_object.cpp
#ifndef partial_write_function_object_hpp_
#define partial_write_function_object_hpp_
#include "partial_write.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<partialWrite>
    partialWriteFunctionObject;
}
#endif
