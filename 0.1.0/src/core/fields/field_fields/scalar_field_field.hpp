#ifndef CORE_FIELDS_FIELD_FIELDS_SCALAR_FIELD_FIELD_HPP_
#define CORE_FIELDS_FIELD_FIELDS_SCALAR_FIELD_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_field.hpp"
#include "scalar.hpp"
#define TEMPLATE template<template<class> class Field>
#include "field_field_functions_m.inc"


namespace mousse {

template<template<class> class Field>
void stabilise
(
  FieldField<Field, scalar>& Res,
  const FieldField<Field, scalar>& sf,
  const scalar s
);

template<template<class> class Field>
tmp<FieldField<Field, scalar> > stabilise
(
  const FieldField<Field, scalar>&,
  const scalar s
);

template<template<class> class Field>
tmp<FieldField<Field, scalar> > stabilise
(
  const tmp<FieldField<Field, scalar> >&,
  const scalar s
);

BINARY_TYPE_OPERATOR(scalar, scalar, scalar, +, add)
BINARY_TYPE_OPERATOR(scalar, scalar, scalar, -, subtract)
BINARY_OPERATOR(scalar, scalar, scalar, *, multiply)
BINARY_OPERATOR(scalar, scalar, scalar, /, divide)
BINARY_TYPE_OPERATOR_SF(scalar, scalar, scalar, /, divide)
BINARY_FUNCTION(scalar, scalar, scalar, pow)
BINARY_TYPE_FUNCTION(scalar, scalar, scalar, pow)
BINARY_FUNCTION(scalar, scalar, scalar, atan2)
BINARY_TYPE_FUNCTION(scalar, scalar, scalar, atan2)
UNARY_FUNCTION(scalar, scalar, pow3)
UNARY_FUNCTION(scalar, scalar, pow4)
UNARY_FUNCTION(scalar, scalar, pow5)
UNARY_FUNCTION(scalar, scalar, pow6)
UNARY_FUNCTION(scalar, scalar, pow025)
UNARY_FUNCTION(scalar, scalar, sqrt)
UNARY_FUNCTION(scalar, scalar, cbrt)
UNARY_FUNCTION(scalar, scalar, sign)
UNARY_FUNCTION(scalar, scalar, pos)
UNARY_FUNCTION(scalar, scalar, neg)
UNARY_FUNCTION(scalar, scalar, posPart)
UNARY_FUNCTION(scalar, scalar, negPart)
UNARY_FUNCTION(scalar, scalar, exp)
UNARY_FUNCTION(scalar, scalar, log)
UNARY_FUNCTION(scalar, scalar, log10)
UNARY_FUNCTION(scalar, scalar, sin)
UNARY_FUNCTION(scalar, scalar, cos)
UNARY_FUNCTION(scalar, scalar, tan)
UNARY_FUNCTION(scalar, scalar, asin)
UNARY_FUNCTION(scalar, scalar, acos)
UNARY_FUNCTION(scalar, scalar, atan)
UNARY_FUNCTION(scalar, scalar, sinh)
UNARY_FUNCTION(scalar, scalar, cosh)
UNARY_FUNCTION(scalar, scalar, tanh)
UNARY_FUNCTION(scalar, scalar, asinh)
UNARY_FUNCTION(scalar, scalar, acosh)
UNARY_FUNCTION(scalar, scalar, atanh)
UNARY_FUNCTION(scalar, scalar, erf)
UNARY_FUNCTION(scalar, scalar, erfc)
UNARY_FUNCTION(scalar, scalar, lgamma)
UNARY_FUNCTION(scalar, scalar, j0)
UNARY_FUNCTION(scalar, scalar, j1)
UNARY_FUNCTION(scalar, scalar, y0)
UNARY_FUNCTION(scalar, scalar, y1)

#define BESSEL_FUNC(func)                                                     \
void func                                                                     \
(                                                                             \
  FieldField<Field, scalar>& Res,                                             \
  const int n,                                                                \
  const FieldField<Field, scalar>& sf                                         \
);                                                                            \
tmp<scalarField> func(const int n, const FieldField<Field, scalar>&);         \
tmp<scalarField> func(const int n, const tmp<FieldField<Field, scalar> >&);

BESSEL_FUNC(jn)
BESSEL_FUNC(yn)

#undef BESSEL_FUNC
}  // namespace mousse

#include "undef_field_functions_m.inc"
#include "scalar_field_field.ipp"

#endif
