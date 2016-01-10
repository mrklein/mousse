// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef vector_2d_field_io_field_hpp_
#define vector_2d_field_io_field_hpp_
#include "vector_2d_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<vector2DField> vector2DFieldIOField;
  typedef CompactIOField<vector2DField, vector2D> vector2DFieldCompactIOField;
}
#endif
