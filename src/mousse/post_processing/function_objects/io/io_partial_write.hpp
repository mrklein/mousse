#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_IO_PARTIAL_WRITE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_IO_PARTIAL_WRITE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "partial_write.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<partialWrite> IOpartialWrite;

}

#endif

