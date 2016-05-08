#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_BLENDING_FACTOR_IO_BLENDING_FACTOR_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_BLENDING_FACTOR_IO_BLENDING_FACTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_factor.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<blendingFactor> IOblendingFactor;

}

#endif

