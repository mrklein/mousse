// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_vorticity_hpp_
#define io_vorticity_hpp_
#include "vorticity.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<vorticity> IOvorticity;
}
#endif
