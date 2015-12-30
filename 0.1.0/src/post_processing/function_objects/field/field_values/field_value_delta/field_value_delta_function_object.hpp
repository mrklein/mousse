// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   field_value_delta_function_object.cpp
#ifndef field_value_delta_function_object_hpp_
#define field_value_delta_function_object_hpp_
#include "field_value_delta.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldValues::fieldValueDelta>
    fieldValueDeltaFunctionObject;
}
#endif
