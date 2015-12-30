// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   wall_bounded_stream_line_function_object.cpp
#ifndef wall_bounded_stream_line_function_object_hpp_
#define wall_bounded_stream_line_function_object_hpp_
#include "wall_bounded_stream_line.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<wallBoundedStreamLine>
    wallBoundedStreamLineFunctionObject;
}
#endif
