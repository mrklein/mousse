// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   cell_source_function_object.cpp
#ifndef cell_source_function_object_hpp_
#define cell_source_function_object_hpp_
#include "cell_source.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldValues::cellSource>
    cellSourceFunctionObject;
}
#endif
