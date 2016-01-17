// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef scalar_field_io_field_hpp_
#define scalar_field_io_field_hpp_
#include "scalar_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<scalarField> scalarFieldIOField;
  typedef CompactIOField<scalarField, scalar> scalarFieldCompactIOField;
}
#endif
