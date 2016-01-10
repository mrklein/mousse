// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef vector_field_io_field_hpp_
#define vector_field_io_field_hpp_
#include "vector_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<vectorField> vectorFieldIOField;
  typedef CompactIOField<vectorField, vector> vectorFieldCompactIOField;
}
#endif
