// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   turbulence_fields_function_object.cpp
#ifndef turbulence_fields_function_object_hpp_
#define turbulence_fields_function_object_hpp_
#include "turbulence_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<turbulenceFields>
    turbulenceFieldsFunctionObject;
}
#endif
