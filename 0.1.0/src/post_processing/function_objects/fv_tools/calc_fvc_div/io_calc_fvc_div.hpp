#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_DIV_IO_CALC_FVC_DIV_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FV_TOOLS_CALC_FVC_DIV_IO_CALC_FVC_DIV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_fvc_div.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<calcFvcDiv> IOcalcFvcDiv;
}
#endif
