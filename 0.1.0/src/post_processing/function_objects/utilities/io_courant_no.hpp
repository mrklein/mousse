#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_IO_COURANT_NO_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_COURANT_NO_IO_COURANT_NO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "courant_no.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<CourantNo> IOCourantNo;

}

#endif

