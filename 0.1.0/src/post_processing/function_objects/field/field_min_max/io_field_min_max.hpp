#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_MIN_MAX_IO_FIELD_MIN_MAX_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_MIN_MAX_IO_FIELD_MIN_MAX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_min_max.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<fieldMinMax> IOfieldMinMax;
}
#endif
