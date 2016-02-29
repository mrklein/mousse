#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_PROCESSOR_FIELD_IO_PROCESSOR_FIELD_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_PROCESSOR_FIELD_IO_PROCESSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_field.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<processorField> IOprocessorField;
}
#endif
