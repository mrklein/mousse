// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef symm_tensor_field_io_field_hpp_
#define symm_tensor_field_io_field_hpp_
#include "symm_tensor_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<symmTensorField> symmTensorFieldIOField;
  typedef CompactIOField<symmTensorField, symmTensor>
  symmTensorFieldCompactIOField;
}
#endif
