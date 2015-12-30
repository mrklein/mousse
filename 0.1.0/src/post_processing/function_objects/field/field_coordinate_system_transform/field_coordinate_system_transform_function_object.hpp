// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   field_coordinate_system_transform_function_object.cpp
#ifndef field_coordinate_system_transform_function_object_hpp_
#define field_coordinate_system_transform_function_object_hpp_
#include "field_coordinate_system_transform.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldCoordinateSystemTransform>
    fieldCoordinateSystemTransformFunctionObject;
}
#endif
