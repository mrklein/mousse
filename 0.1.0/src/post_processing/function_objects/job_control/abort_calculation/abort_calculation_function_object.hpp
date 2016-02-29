#ifndef POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_ABORT_CALCULATION_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_ABORT_CALCULATION_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   abort_calculation_function_object.cpp
#include "abort_calculation.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<abortCalculation>
    abortCalculationFunctionObject;
}
#endif
