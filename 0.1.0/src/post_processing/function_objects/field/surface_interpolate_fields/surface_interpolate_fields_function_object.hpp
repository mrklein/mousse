// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   surface_interpolate_fields_function_object.cpp
#ifndef surface_interpolate_fields_function_object_hpp_
#define surface_interpolate_fields_function_object_hpp_
#include "surface_interpolate_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<surfaceInterpolateFields>
    surfaceInterpolateFieldsFunctionObject;
}
#endif
