// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dsmc_fields_function_object.cpp
#ifndef dsmc_fields_function_object_hpp_
#define dsmc_fields_function_object_hpp_
#include "dsmc_fields.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<dsmcFields>
    dsmcFieldsFunctionObject;
}
#endif
