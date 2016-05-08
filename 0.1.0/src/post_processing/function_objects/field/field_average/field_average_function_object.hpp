#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_AVERAGE_FIELD_AVERAGE_FUNCTION_OBJECT_FIELD_AVERAGE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_AVERAGE_FIELD_AVERAGE_FUNCTION_OBJECT_FIELD_AVERAGE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_average.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<fieldAverage>
  fieldAverageFunctionObject;

}

#endif

