// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_system_call_hpp_
#define io_system_call_hpp_
#include "system_call.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<systemCall> IOsystemCall;
}
#endif
