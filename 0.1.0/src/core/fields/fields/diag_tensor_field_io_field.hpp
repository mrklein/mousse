// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef diag_tensor_field_io_field_hpp_
#define diag_tensor_field_io_field_hpp_
#include "diag_tensor_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<diagTensorField> diagTensorFieldIOField;
  typedef CompactIOField<diagTensorField, diagTensor>
  diagTensorFieldCompactIOField;
}
#endif
