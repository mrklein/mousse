// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   forces_function_object.cpp
#ifndef forces_function_object_hpp_
#define forces_function_object_hpp_
#include "forces.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<forces> forcesFunctionObject;
}
#endif
