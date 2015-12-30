// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_function_object_template_hpp_
#define io_function_object_template_hpp_
#include "function_object_template.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<${typeName}FunctionObject>
  IO${typeName}FunctionObject;
}
#endif
