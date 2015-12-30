// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   residuals_function_object.cpp
#ifndef residuals_function_object_hpp_
#define residuals_function_object_hpp_
#include "residuals.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<residuals>
    residualsFunctionObject;
}
#endif
