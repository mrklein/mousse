#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_NEAR_WALL_FIELDS_IO_NEAR_WALL_FIELDS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_NEAR_WALL_FIELDS_IO_NEAR_WALL_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_fields.hpp"
#include "io_output_filter.hpp"


namespace mousse {

typedef IOOutputFilter<nearWallFields> IOnearWallFields;

}

#endif

