#ifndef POST_PROCESSING_FUNCTION_OBJECTS_SYSTEM_CALL_IO_SYSTEM_CALL_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_SYSTEM_CALL_IO_SYSTEM_CALL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "system_call.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<systemCall> IOsystemCall;
}
#endif
