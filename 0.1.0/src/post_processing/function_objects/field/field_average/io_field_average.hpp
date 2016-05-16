#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_AVERAGE_FIELD_IO_FIELD_AVERAGE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_AVERAGE_FIELD_IO_FIELD_AVERAGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_average.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<fieldAverage> IOFieldAverage;

}

#endif

