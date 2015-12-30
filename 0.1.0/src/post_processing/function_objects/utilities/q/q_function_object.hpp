// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   q_function_object.cpp
#ifndef q_function_object_hpp_
#define q_function_object_hpp_
#include "q.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<Q> QFunctionObject;
}
#endif
