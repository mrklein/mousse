// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_field_field.hpp"
#define TEMPLATE template<template<class> class Field>
#include "field_field_functions_m.ipp"


namespace mousse {

template<template<class> class Field>
void stabilise
(
  FieldField<Field, scalar>& f,
  const FieldField<Field, scalar>& f1,
  const scalar s
)
{
  FOR_ALL(f, i) {
    stabilise(f[i], f1[i], s);
  }
}


template<template<class> class Field>
tmp<FieldField<Field, scalar>> stabilise
(
  const FieldField<Field, scalar>& f1,
  const scalar s
)
{
  tmp<FieldField<Field, scalar>> tf
  {
    FieldField<Field, scalar>::NewCalculatedType(f1)
  };
  stabilise(tf(), f1, s);
  return tf;
}


template<template<class> class Field>
tmp<FieldField<Field, scalar>> stabilise
(
  const tmp<FieldField<Field, scalar>>& tf1,
  const scalar s
)
{
  tmp<FieldField<Field, scalar>> tf(tf1.ptr());
  stabilise(tf(), tf(), s);
  return tf;
}

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
                                                                              \
template<template<class> class Field>                                         \
void func                                                                     \
(                                                                             \
  FieldField<Field, scalar>& res,                                             \
  const int n,                                                                \
  const FieldField<Field, scalar>& sf                                         \
)                                                                             \
{                                                                             \
  FOR_ALL(res, i)                                                             \
  {                                                                           \
    func(res[i], n, sf[i]);                                                   \
  }                                                                           \
}                                                                             \
                                                                              \
template<template<class> class Field>                                         \
tmp<FieldField<Field, scalar>> func                                           \
(                                                                             \
  const int n,                                                                \
  const FieldField<Field, scalar>& sf                                         \
)                                                                             \
{                                                                             \
  tmp<FieldField<Field, scalar>> tRes                                         \
  {                                                                           \
    FieldField<Field, scalar>::NewCalculatedType(sf)                          \
  };                                                                          \
  func(tRes(), n, sf);                                                        \
  return tRes;                                                                \
}                                                                             \
                                                                              \
template<template<class> class Field>                                         \
tmp<FieldField<Field, scalar>> func                                           \
(                                                                             \
  const int n,                                                                \
  const tmp<FieldField<Field, scalar>>& tsf                                   \
)                                                                             \
{                                                                             \
  tmp<FieldField<Field, scalar>> tRes                                         \
  {                                                                           \
    reuseTmpFieldField<Field, scalar, scalar>::New(tsf)                       \
  };                                                                          \
  func(tRes(), n, tsf());                                                     \
  reuseTmpFieldField<Field, scalar, scalar>::clear(tsf);                      \
  return tRes;                                                                \
}

BESSEL_FUNC(jn)
BESSEL_FUNC(yn)

#undef BESSEL_FUNC

}  // namespace mousse

#include "undef_field_functions_m.inc"
