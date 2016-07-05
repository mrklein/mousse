#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SCALAR_TRANSPORT_IO_SCALAR_TRANSPORT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SCALAR_TRANSPORT_IO_SCALAR_TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_transport.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<scalarTransport> IOscalarTransport;

}

#endif

