#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_SURFACES_FUNCTION_OBJECT_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_SURFACES_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   sampled_surfaces_function_object.cpp
#include "sampled_surfaces.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<sampledSurfaces>
    sampledSurfacesFunctionObject;
}
#endif
