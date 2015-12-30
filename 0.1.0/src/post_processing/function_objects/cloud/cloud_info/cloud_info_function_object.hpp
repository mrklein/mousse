// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   cloud_info_function_object.cpp
#ifndef cloud_info_function_object_hpp_
#define cloud_info_function_object_hpp_
#include "cloud_info.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<cloudInfo>
    cloudInfoFunctionObject;
}
#endif
