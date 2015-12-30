// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_surface_interpolate_fields_hpp_
#define io_surface_interpolate_fields_hpp_
#include "surface_interpolate_fields.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<surfaceInterpolateFields> IOsurfaceInterpolateFields;
}
#endif
