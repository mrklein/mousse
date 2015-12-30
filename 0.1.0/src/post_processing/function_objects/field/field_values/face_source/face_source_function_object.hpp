// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   face_source_function_object.cpp
#ifndef face_source_function_object_hpp_
#define face_source_function_object_hpp_
#include "face_source.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<fieldValues::faceSource>
    faceSourceFunctionObject;
}
#endif
