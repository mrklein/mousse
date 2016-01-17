// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef tensor_field_io_field_hpp_
#define tensor_field_io_field_hpp_
#include "tensor_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<tensorField> tensorFieldIOField;
  typedef CompactIOField<tensorField, tensor> tensorFieldCompactIOField;
}
#endif
