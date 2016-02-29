#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_STREAM_LINE_STREAM_LINE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   stream_line_function_object.cpp
#include "stream_line.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<streamLine>
    streamLineFunctionObject;
}
#endif
