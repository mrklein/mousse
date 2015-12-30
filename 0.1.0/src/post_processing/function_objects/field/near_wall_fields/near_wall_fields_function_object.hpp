// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   near_wall_fields_function_object.cpp
#ifndef near_wall_fields_function_object_hpp_
#define near_wall_fields_function_object_hpp_
#include "near_wall_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<nearWallFields>
    nearWallFieldsFunctionObject;
}
#endif
