// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   y_plus_function_object.cpp
#ifndef y_plus_function_object_hpp_
#define y_plus_function_object_hpp_
#include "y_plus.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<yPlus> yPlusFunctionObject;
}
#endif
