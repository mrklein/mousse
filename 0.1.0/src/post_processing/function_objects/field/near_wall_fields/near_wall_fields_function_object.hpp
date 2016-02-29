#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_NEAR_WALL_FIELDS_NEAR_WALL_FIELDS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_NEAR_WALL_FIELDS_NEAR_WALL_FIELDS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   near_wall_fields_function_object.cpp
#include "near_wall_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<nearWallFields>
    nearWallFieldsFunctionObject;
}
#endif
