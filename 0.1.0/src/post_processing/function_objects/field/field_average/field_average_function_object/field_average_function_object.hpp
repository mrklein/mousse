// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   field_average_function_object.cpp
#ifndef field_average_function_object_hpp_
#define field_average_function_object_hpp_
#include "field_average.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldAverage>
    fieldAverageFunctionObject;
}
#endif
