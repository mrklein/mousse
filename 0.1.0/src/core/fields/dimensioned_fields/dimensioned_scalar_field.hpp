#ifndef CORE_FIELDS_DIMENSIONED_FIELDS_DIMENSIONED_SCALAR_FIELD_HPP_
#define CORE_FIELDS_DIMENSIONED_FIELDS_DIMENSIONED_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_scalar_field.cpp
#include "dimensioned_field.hpp"
#include "scalar.hpp"
#define TEMPLATE template<class GeoMesh>
#include "dimensioned_field_functions_m.hpp"
namespace mousse
{
template<class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > stabilise
(
  const DimensionedField<scalar, GeoMesh>&,
  const dimensioned<scalar>&
);
template<class GeoMesh>
tmp<DimensionedField<scalar, GeoMesh> > stabilise
(
  const tmp<DimensionedField<scalar, GeoMesh> >&,
  const dimensioned<scalar>&
);
BINARY_TYPE_OPERATOR(scalar, scalar, scalar, +, '+', add)
BINARY_TYPE_OPERATOR(scalar, scalar, scalar, -, '-', subtract)
BINARY_OPERATOR(scalar, scalar, scalar, *, '*', multiply)
BINARY_OPERATOR(scalar, scalar, scalar, /, '|', divide)
BINARY_TYPE_OPERATOR_SF(scalar, scalar, scalar, /, '|', divide)
BINARY_FUNCTION(scalar, scalar, scalar, pow)
BINARY_TYPE_FUNCTION(scalar, scalar, scalar, pow)
BINARY_FUNCTION(scalar, scalar, scalar, atan2)
BINARY_TYPE_FUNCTION(scalar, scalar, scalar, atan2)
UNARY_FUNCTION(scalar, scalar, pow3, pow3)
UNARY_FUNCTION(scalar, scalar, pow4, pow4)
UNARY_FUNCTION(scalar, scalar, pow5, pow5)
UNARY_FUNCTION(scalar, scalar, pow6, pow6)
UNARY_FUNCTION(scalar, scalar, pow025, pow025)
UNARY_FUNCTION(scalar, scalar, sqrt, sqrt)
UNARY_FUNCTION(scalar, scalar, cbrt, cbrt)
UNARY_FUNCTION(scalar, scalar, sign, sign)
UNARY_FUNCTION(scalar, scalar, pos, pos)
UNARY_FUNCTION(scalar, scalar, neg, neg)
UNARY_FUNCTION(scalar, scalar, posPart, posPart)
UNARY_FUNCTION(scalar, scalar, negPart, negPart)
UNARY_FUNCTION(scalar, scalar, exp, trans)
UNARY_FUNCTION(scalar, scalar, log, trans)
UNARY_FUNCTION(scalar, scalar, log10, trans)
UNARY_FUNCTION(scalar, scalar, sin, trans)
UNARY_FUNCTION(scalar, scalar, cos, trans)
UNARY_FUNCTION(scalar, scalar, tan, trans)
UNARY_FUNCTION(scalar, scalar, asin, trans)
UNARY_FUNCTION(scalar, scalar, acos, trans)
UNARY_FUNCTION(scalar, scalar, atan, trans)
UNARY_FUNCTION(scalar, scalar, sinh, trans)
UNARY_FUNCTION(scalar, scalar, cosh, trans)
UNARY_FUNCTION(scalar, scalar, tanh, trans)
UNARY_FUNCTION(scalar, scalar, asinh, trans)
UNARY_FUNCTION(scalar, scalar, acosh, trans)
UNARY_FUNCTION(scalar, scalar, atanh, trans)
UNARY_FUNCTION(scalar, scalar, erf, trans)
UNARY_FUNCTION(scalar, scalar, erfc, trans)
UNARY_FUNCTION(scalar, scalar, lgamma, trans)
UNARY_FUNCTION(scalar, scalar, j0, trans)
UNARY_FUNCTION(scalar, scalar, j1, trans)
UNARY_FUNCTION(scalar, scalar, y0, trans)
UNARY_FUNCTION(scalar, scalar, y1, trans)
#define BesselFunc(func)                                                       \
                                       \
template<class GeoMesh>                                                        \
tmp<DimensionedField<scalar, GeoMesh> > func                                   \
(                                                                              \
  const int n,                                                               \
  const DimensionedField<scalar, GeoMesh>&                                   \
);                                                                             \
                                       \
template<class GeoMesh>                                                        \
tmp<DimensionedField<scalar, GeoMesh> > func                                   \
(                                                                              \
  const int n,                                                               \
  const tmp<DimensionedField<scalar, GeoMesh> >&                             \
);
BesselFunc(jn)
BesselFunc(yn)
#undef BesselFunc
}  // namespace mousse
#include "undef_field_functions_m.hpp"
#ifdef NoRepository
#   include "dimensioned_scalar_field.cpp"
#endif
#endif
