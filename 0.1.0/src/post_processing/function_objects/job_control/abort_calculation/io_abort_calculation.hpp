// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_abort_calculation_hpp_
#define io_abort_calculation_hpp_
#include "abort_calculation.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<abortCalculation> IOabortCalculation;
}
#endif
