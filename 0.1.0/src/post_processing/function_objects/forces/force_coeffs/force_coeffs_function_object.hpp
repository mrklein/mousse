// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   force_coeffs_function_object.cpp
#ifndef force_coeffs_function_object_hpp_
#define force_coeffs_function_object_hpp_
#include "force_coeffs.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<forceCoeffs> forceCoeffsFunctionObject;
}
#endif
