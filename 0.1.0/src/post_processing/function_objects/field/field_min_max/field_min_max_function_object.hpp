#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_MIN_MAX_FIELD_MIN_MAX_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_MIN_MAX_FIELD_MIN_MAX_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   field_min_max_function_object.cpp
#include "field_min_max.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldMinMax>
    fieldMinMaxFunctionObject;
}
#endif
