// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_field.hpp"
#include "unit_conversion.hpp"
#define TEMPLATE
#include "field_functions_m.ipp"


namespace mousse {

template<>
tmp<scalarField> scalarField::component(const direction) const
{
  return *this;
}
void component(scalarField& sf, const UList<scalar>& f, const direction)
{
  sf = f;
}


template<>
void scalarField::replace(const direction, const UList<scalar>& sf)
{
  *this = sf;
}


template<>
void scalarField::replace(const direction, const scalar& s)
{
  *this = s;
}


void stabilise(scalarField& res, const UList<scalar>& sf, const scalar s)
{
  TFOR_ALL_F_OP_FUNC_S_F
  (
    scalar, res, =, ::mousse::stabilise, scalar, s, scalar, sf
  )
}


tmp<scalarField> stabilise(const UList<scalar>& sf, const scalar s)
{
  tmp<scalarField> tRes{new scalarField{sf.size()}};
  stabilise(tRes(), sf, s);
  return tRes;
}


tmp<scalarField> stabilise(const tmp<scalarField>& tsf, const scalar s)
{
  tmp<scalarField> tRes = reuseTmp<scalar, scalar>::New(tsf);
  stabilise(tRes(), tsf(), s);
  reuseTmp<scalar, scalar>::clear(tsf);
  return tRes;
}


template<>
scalar sumProd(const UList<scalar>& f1, const UList<scalar>& f2)
{
  if (f1.size() && (f1.size() == f2.size())) {
    scalar SumProd = 0.0;
    TFOR_ALL_S_OP_F_OP_F(scalar, SumProd, +=, scalar, f1, *, scalar, f2)
    return SumProd;
  } else {
    return 0.0;
  }
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
UNARY_FUNCTION(scalar, scalar, degToRad)
UNARY_FUNCTION(scalar, scalar, radToDeg)
UNARY_FUNCTION(scalar, scalar, atmToPa)
UNARY_FUNCTION(scalar, scalar, paToAtm)

#define BESSEL_FUNC(func)                                                     \
void func(scalarField& res, const int n, const UList<scalar>& sf)             \
{                                                                             \
  TFOR_ALL_F_OP_FUNC_S_F(scalar, res, =, ::mousse::func, int, n, scalar, sf)  \
}                                                                             \
                                                                              \
tmp<scalarField> func(const int n, const UList<scalar>& sf)                   \
{                                                                             \
  tmp<scalarField> tRes{new scalarField{sf.size()}};                          \
  func(tRes(), n, sf);                                                        \
  return tRes;                                                                \
}                                                                             \
                                                                              \
tmp<scalarField> func(const int n, const tmp<scalarField>& tsf)               \
{                                                                             \
  tmp<scalarField> tRes = reuseTmp<scalar, scalar>::New(tsf);                 \
  func(tRes(), n, tsf());                                                     \
  reuseTmp<scalar, scalar>::clear(tsf);                                       \
  return tRes;                                                                \
}

BESSEL_FUNC(jn)
BESSEL_FUNC(yn)

#undef BESSEL_FUNC

}  // namespace mousse

#include "undef_field_functions_m.inc"
