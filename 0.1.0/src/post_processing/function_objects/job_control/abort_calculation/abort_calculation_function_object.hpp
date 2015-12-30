// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   abort_calculation_function_object.cpp
#ifndef abort_calculation_function_object_hpp_
#define abort_calculation_function_object_hpp_
#include "abort_calculation.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<abortCalculation>
    abortCalculationFunctionObject;
}
#endif
