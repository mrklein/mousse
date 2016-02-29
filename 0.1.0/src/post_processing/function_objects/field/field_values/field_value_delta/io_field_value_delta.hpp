#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_DELTA_IO_FIELD_VALUE_DELTA_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FIELD_VALUE_DELTA_IO_FIELD_VALUE_DELTA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value_delta.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<fieldValueDelta> IOfieldValueDelta;
}
#endif
