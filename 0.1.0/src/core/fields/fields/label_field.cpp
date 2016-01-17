// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_field.hpp"
#define TEMPLATE
#include "field_functions_m.cpp"

namespace mousse
{

BINARY_TYPE_OPERATOR(label, label, label, +, add)
BINARY_TYPE_OPERATOR(label, label, label, -, subtract)

template<>
tmp<labelField> labelField::component(const direction) const
{
  return *this;
}
template<>
void component
(
  labelField& lf,
  const labelUList& f,
  const direction
)
{
  lf = f;
}
template<>
void labelField::replace(const direction, const labelUList& lf)
{
  *this = lf;
}
}  // namespace mousse

#include "undef_field_functions_m.hpp"
