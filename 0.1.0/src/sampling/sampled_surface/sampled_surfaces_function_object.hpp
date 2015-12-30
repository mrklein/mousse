// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   sampled_surfaces_function_object.cpp
#ifndef sampled_surfaces_function_object_hpp_
#define sampled_surfaces_function_object_hpp_
#include "sampled_surfaces.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<sampledSurfaces>
    sampledSurfacesFunctionObject;
}
#endif
