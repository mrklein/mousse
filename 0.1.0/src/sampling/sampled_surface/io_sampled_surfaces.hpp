#ifndef SAMPLING_SAMPLED_SURFACE_IO_SAMPLED_SURFACES_HPP_
#define SAMPLING_SAMPLED_SURFACE_IO_SAMPLED_SURFACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surfaces.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<sampledSurfaces> IOsampledSurfaces;
}
#endif
