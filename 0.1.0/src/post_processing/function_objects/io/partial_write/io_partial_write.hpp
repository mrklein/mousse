// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_partial_write_hpp_
#define io_partial_write_hpp_
#include "partial_write.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<partialWrite> IOpartialWrite;
}
#endif
