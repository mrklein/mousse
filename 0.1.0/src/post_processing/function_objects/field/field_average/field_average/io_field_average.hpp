// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef IOfieldAverage_H
#define IOfieldAverage_H
#include "field_average.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<fieldAverage> IOFieldAverage;
}
#endif
