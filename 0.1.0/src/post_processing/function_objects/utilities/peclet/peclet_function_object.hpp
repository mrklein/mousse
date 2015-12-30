// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   peclet_function_object.cpp
#ifndef peclet_function_object_hpp_
#define peclet_function_object_hpp_
#include "peclet.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<Peclet> PecletFunctionObject;
}
#endif
