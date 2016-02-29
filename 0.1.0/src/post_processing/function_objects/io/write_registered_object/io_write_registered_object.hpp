#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_REGISTERED_OBJECT_IO_WRITE_REGISTERED_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_WRITE_REGISTERED_OBJECT_IO_WRITE_REGISTERED_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_registered_object.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<writeRegisteredObject> IOwriteRegisteredObject;
}
#endif
