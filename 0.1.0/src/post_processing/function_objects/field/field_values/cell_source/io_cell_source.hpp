// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_cell_source_hpp_
#define io_cell_source_hpp_
#include "cell_source.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<cellSource> IOcellSource;
}
#endif
