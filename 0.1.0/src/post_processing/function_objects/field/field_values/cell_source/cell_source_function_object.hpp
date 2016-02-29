#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_CELL_SOURCE_CELL_SOURCE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_CELL_SOURCE_CELL_SOURCE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   cell_source_function_object.cpp
#include "cell_source.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldValues::cellSource>
    cellSourceFunctionObject;
}
#endif
