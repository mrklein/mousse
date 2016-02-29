#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_CELL_SOURCE_IO_CELL_SOURCE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_CELL_SOURCE_IO_CELL_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_source.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<cellSource> IOcellSource;
}
#endif
