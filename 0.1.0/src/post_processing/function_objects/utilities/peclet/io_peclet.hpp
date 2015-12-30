// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_peclet_hpp_
#define io_peclet_hpp_
#include "peclet.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<Peclet> IOPeclet;
}
#endif
