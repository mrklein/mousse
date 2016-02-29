#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_WALL_BOUNDED_STREAM_LINE_WALL_BOUNDED_STREAM_LINE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_WALL_BOUNDED_STREAM_LINE_WALL_BOUNDED_STREAM_LINE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   wall_bounded_stream_line_function_object.cpp
#include "wall_bounded_stream_line.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<wallBoundedStreamLine>
    wallBoundedStreamLineFunctionObject;
}
#endif
