#ifndef CORE_FIELDS_FIELDS_LABEL_FIELD_IO_FIELD_HPP_
#define CORE_FIELDS_FIELDS_LABEL_FIELD_IO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_field.hpp"
#include "iofield.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<labelField> labelFieldIOField;
  typedef CompactIOField<labelField, label> labelFieldCompactIOField;
}
#endif
