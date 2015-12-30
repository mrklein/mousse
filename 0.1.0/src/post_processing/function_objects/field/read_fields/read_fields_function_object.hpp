// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_fields_function_object.cpp
#ifndef read_fields_function_object_hpp_
#define read_fields_function_object_hpp_
#include "read_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<readFields>
    readFieldsFunctionObject;
}
#endif
