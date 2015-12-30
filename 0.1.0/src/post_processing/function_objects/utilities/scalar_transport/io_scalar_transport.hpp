// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_scalar_transport_hpp_
#define io_scalar_transport_hpp_
#include "scalar_transport.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<scalarTransport> IOscalarTransport;
}
#endif
