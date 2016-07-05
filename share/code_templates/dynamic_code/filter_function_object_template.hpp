// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   filter_function_object_template.cpp
#ifndef filter_function_object_template_hpp_
#define filter_function_object_template_hpp_
#include "function_object_template.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<${typeName}FunctionObject>
    ${typeName}FilterFunctionObject;
}
#endif
