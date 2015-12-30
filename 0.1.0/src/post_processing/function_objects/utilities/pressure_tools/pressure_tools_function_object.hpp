// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   pressure_tools_function_object.cpp
#ifndef pressure_tools_function_object_hpp_
#define pressure_tools_function_object_hpp_
#include "pressure_tools.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<pressureTools>
    pressureToolsFunctionObject;
}
#endif
