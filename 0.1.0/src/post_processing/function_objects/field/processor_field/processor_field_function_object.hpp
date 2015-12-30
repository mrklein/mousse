// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   processor_field_function_object.cpp
#ifndef processor_field_function_object_hpp_
#define processor_field_function_object_hpp_
#include "processor_field.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<processorField>
    processorFieldFunctionObject;
}
#endif
