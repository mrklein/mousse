// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef spherical_tensor_field_io_field_hpp_
#define spherical_tensor_field_io_field_hpp_
#include "spherical_tensor_field.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<sphericalTensorField> sphericalTensorFieldIOField;
  typedef CompactIOField<sphericalTensorField, sphericalTensor>
  sphericalTensorFieldCompactIOField;
}
#endif
