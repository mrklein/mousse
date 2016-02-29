#ifndef POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_IO_ABORT_CALCULATION_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_JOB_CONTROL_ABORT_CALCULATION_IO_ABORT_CALCULATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "abort_calculation.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<abortCalculation> IOabortCalculation;
}
#endif
