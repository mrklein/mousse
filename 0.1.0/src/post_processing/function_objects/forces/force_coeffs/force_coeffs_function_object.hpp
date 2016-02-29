#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FORCES_FORCE_COEFFS_FORCE_COEFFS_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FORCES_FORCE_COEFFS_FORCE_COEFFS_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   force_coeffs_function_object.cpp
#include "force_coeffs.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<forceCoeffs> forceCoeffsFunctionObject;
}
#endif
