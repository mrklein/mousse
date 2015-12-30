// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_field_coordinate_system_transform_hpp_
#define io_field_coordinate_system_transform_hpp_
#include "field_coordinate_system_transform.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<fieldCoordinateSystemTransform>
    IOfieldCoordinateSystemTransform;
}
#endif
