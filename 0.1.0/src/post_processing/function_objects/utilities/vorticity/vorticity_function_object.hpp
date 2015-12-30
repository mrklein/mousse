// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   vorticity_function_object.cpp
#ifndef vorticity_function_object_hpp_
#define vorticity_function_object_hpp_
#include "vorticity.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<vorticity> vorticityFunctionObject;
}
#endif
