// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_near_wall_fields_hpp_
#define io_near_wall_fields_hpp_
#include "near_wall_fields.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<nearWallFields> IOnearWallFields;
}
#endif
