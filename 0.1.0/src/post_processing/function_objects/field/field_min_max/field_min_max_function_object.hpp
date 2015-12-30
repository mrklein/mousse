// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   field_min_max_function_object.cpp
#ifndef field_min_max_function_object_hpp_
#define field_min_max_function_object_hpp_
#include "field_min_max.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldMinMax>
    fieldMinMaxFunctionObject;
}
#endif
