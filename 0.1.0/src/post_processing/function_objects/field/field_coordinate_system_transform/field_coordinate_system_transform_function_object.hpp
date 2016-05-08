#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_COORDINATE_SYSTEM_TRANSFORM_FIELD_COORDINATE_SYSTEM_TRANSFORM_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_COORDINATE_SYSTEM_TRANSFORM_FIELD_COORDINATE_SYSTEM_TRANSFORM_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_coordinate_system_transform.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<fieldCoordinateSystemTransform>
  fieldCoordinateSystemTransformFunctionObject;

}

#endif

