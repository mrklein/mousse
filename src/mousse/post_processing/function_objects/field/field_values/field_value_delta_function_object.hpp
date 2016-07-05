#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_DELTA_FIELD_VALUE_DELTA_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_DELTA_FIELD_VALUE_DELTA_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value_delta.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<fieldValues::fieldValueDelta>
  fieldValueDeltaFunctionObject;

}

#endif

