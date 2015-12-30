// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   courant_no_function_object.cpp
#ifndef courant_no_function_object_hpp_
#define courant_no_function_object_hpp_
#include "courant_no.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<CourantNo> CourantNoFunctionObject;
}
#endif
