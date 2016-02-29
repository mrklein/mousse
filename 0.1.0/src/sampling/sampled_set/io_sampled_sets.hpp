#ifndef SAMPLING_SAMPLED_SET_IO_SAMPLED_SETS_HPP_
#define SAMPLING_SAMPLED_SET_IO_SAMPLED_SETS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<sampledSets> IOsampledSets;
}
#endif
