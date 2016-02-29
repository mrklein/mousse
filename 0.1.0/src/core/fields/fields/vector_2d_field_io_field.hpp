#ifndef CORE_FIELDS_FIELDS_VECTOR_2D_FIELD_IO_FIELD_HPP_
#define CORE_FIELDS_FIELDS_VECTOR_2D_FIELD_IO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector_2d_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<vector2DField> vector2DFieldIOField;
  typedef CompactIOField<vector2DField, vector2D> vector2DFieldCompactIOField;
}
#endif
