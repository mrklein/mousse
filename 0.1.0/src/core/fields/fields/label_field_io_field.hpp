// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef label_field_io_field_hpp_
#define label_field_io_field_hpp_
#include "label_field.hpp"
#include "iofield.hpp"
#include "compact_io_field.hpp"
namespace mousse
{
  typedef IOField<labelField> labelFieldIOField;
  typedef CompactIOField<labelField, label> labelFieldCompactIOField;
}
#endif
