#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TURBULENCE_FIELDS_TURBULENCE_FIELDS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TURBULENCE_FIELDS_TURBULENCE_FIELDS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   turbulence_fields_function_object.cpp
#include "turbulence_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<turbulenceFields>
    turbulenceFieldsFunctionObject;
}
#endif
