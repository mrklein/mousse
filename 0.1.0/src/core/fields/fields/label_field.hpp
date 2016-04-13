#ifndef CORE_FIELDS_FIELDS_LABEL_FIELD_HPP_
#define CORE_FIELDS_FIELDS_LABEL_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label.hpp"
#include "field.hpp"
#define TEMPLATE
#include "field_functions_m.inc"


namespace mousse {

typedef Field<label> labelField;

BINARY_TYPE_OPERATOR(label, label, label, +, add)
BINARY_TYPE_OPERATOR(label, label, label, -, subtract)

template<>
tmp<labelField> labelField::component(const direction) const;

template<>
void component
(
  labelField& lf,
  const labelUList& f,
  const direction
);

template<>
void labelField::replace(const direction, const labelUList& lf);

}  // namespace mousse

#include "undef_field_functions_m.inc"

#endif
