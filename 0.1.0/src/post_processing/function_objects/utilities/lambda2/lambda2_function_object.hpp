// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   lambda2_function_object.cpp
#ifndef lambda2_function_object_hpp_
#define lambda2_function_object_hpp_
#include "lambda2.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<Lambda2> Lambda2FunctionObject;
}
#endif
