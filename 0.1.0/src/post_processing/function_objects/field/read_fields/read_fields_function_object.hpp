#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_READ_FIELDS_READ_FIELDS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_READ_FIELDS_READ_FIELDS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_fields_function_object.cpp
#include "read_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<readFields>
    readFieldsFunctionObject;
}
#endif
