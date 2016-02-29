#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_RESIDUALS_IO_RESIDUALS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_RESIDUALS_IO_RESIDUALS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "residuals.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<residuals> IOresiduals;
}
#endif
