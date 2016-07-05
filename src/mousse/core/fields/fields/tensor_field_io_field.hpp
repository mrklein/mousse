#ifndef CORE_FIELDS_FIELDS_TENSOR_FIELD_IO_FIELD_HPP_
#define CORE_FIELDS_FIELDS_TENSOR_FIELD_IO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tensor_field.hpp"
#include "compact_io_field.hpp"


namespace mousse {

typedef IOField<tensorField> tensorFieldIOField;
typedef CompactIOField<tensorField, tensor> tensorFieldCompactIOField;

}
#endif
