#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_COORDINATE_SYSTEM_TRANSFORM_IO_FIELD_COORDINATE_SYSTEM_TRANSFORM_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_COORDINATE_SYSTEM_TRANSFORM_IO_FIELD_COORDINATE_SYSTEM_TRANSFORM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_coordinate_system_transform.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<fieldCoordinateSystemTransform>
    IOfieldCoordinateSystemTransform;
}
#endif
