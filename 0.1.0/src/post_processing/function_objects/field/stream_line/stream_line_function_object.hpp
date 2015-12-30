// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   stream_line_function_object.cpp
#ifndef stream_line_function_object_hpp_
#define stream_line_function_object_hpp_
#include "stream_line.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<streamLine>
    streamLineFunctionObject;
}
#endif
