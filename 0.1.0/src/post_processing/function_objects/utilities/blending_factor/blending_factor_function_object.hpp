// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   blending_factor_function_object.cpp
#ifndef blending_factor_function_object_hpp_
#define blending_factor_function_object_hpp_
#include "blending_factor.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<blendingFactor>
    blendingFactorFunctionObject;
}
#endif
