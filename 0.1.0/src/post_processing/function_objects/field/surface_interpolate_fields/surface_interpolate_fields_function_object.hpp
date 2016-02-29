#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_SURFACE_INTERPOLATE_FIELDS_SURFACE_INTERPOLATE_FIELDS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_SURFACE_INTERPOLATE_FIELDS_SURFACE_INTERPOLATE_FIELDS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   surface_interpolate_fields_function_object.cpp
#include "surface_interpolate_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<surfaceInterpolateFields>
    surfaceInterpolateFieldsFunctionObject;
}
#endif
