#ifndef CORE_FIELDS_FIELDS_SYMM_TENSOR_FIELD_IO_FIELD_HPP_
#define CORE_FIELDS_FIELDS_SYMM_TENSOR_FIELD_IO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symm_tensor_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<symmTensorField> symmTensorFieldIOField;
  typedef CompactIOField<symmTensorField, symmTensor>
  symmTensorFieldCompactIOField;
}
#endif
