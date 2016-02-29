#ifndef CORE_DIMENSIONED_TYPES_DIMENSIONED_SCALAR_HPP_
#define CORE_DIMENSIONED_TYPES_DIMENSIONED_SCALAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   dimensioned_scalar.cpp
#include "dimensioned_type.hpp"
#include "scalar.hpp"
#include "dimensioned_scalar_fwd.hpp"
namespace mousse
{
dimensionedScalar operator+(const dimensionedScalar&, const scalar);
dimensionedScalar operator+(const scalar, const dimensionedScalar&);
dimensionedScalar operator-(const dimensionedScalar&, const scalar);
dimensionedScalar operator-(const scalar, const dimensionedScalar&);
dimensionedScalar operator*(const dimensionedScalar&, const scalar);
dimensionedScalar operator/(const scalar, const dimensionedScalar&);
dimensionedScalar pow(const dimensionedScalar&, const dimensionedScalar&);
dimensionedScalar pow3(const dimensionedScalar&);
dimensionedScalar pow4(const dimensionedScalar&);
dimensionedScalar pow5(const dimensionedScalar&);
dimensionedScalar pow6(const dimensionedScalar&);
dimensionedScalar pow025(const dimensionedScalar&);
dimensionedScalar sqrt(const dimensionedScalar&);
dimensionedScalar cbrt(const dimensionedScalar&);
dimensionedScalar hypot(const dimensionedScalar&, const dimensionedScalar&);
dimensionedScalar sign(const dimensionedScalar&);
dimensionedScalar pos(const dimensionedScalar&);
dimensionedScalar neg(const dimensionedScalar&);
dimensionedScalar posPart(const dimensionedScalar&);
dimensionedScalar negPart(const dimensionedScalar&);
dimensionedScalar exp(const dimensionedScalar&);
dimensionedScalar log(const dimensionedScalar&);
dimensionedScalar log10(const dimensionedScalar&);
dimensionedScalar sin(const dimensionedScalar&);
dimensionedScalar cos(const dimensionedScalar&);
dimensionedScalar tan(const dimensionedScalar&);
dimensionedScalar asin(const dimensionedScalar&);
dimensionedScalar acos(const dimensionedScalar&);
dimensionedScalar atan(const dimensionedScalar&);
dimensionedScalar atan2(const dimensionedScalar&, const dimensionedScalar&);
dimensionedScalar sinh(const dimensionedScalar&);
dimensionedScalar cosh(const dimensionedScalar&);
dimensionedScalar tanh(const dimensionedScalar&);
dimensionedScalar asinh(const dimensionedScalar&);
dimensionedScalar acosh(const dimensionedScalar&);
dimensionedScalar atanh(const dimensionedScalar&);
dimensionedScalar erf(const dimensionedScalar&);
dimensionedScalar erfc(const dimensionedScalar&);
dimensionedScalar lgamma(const dimensionedScalar&);
dimensionedScalar j0(const dimensionedScalar&);
dimensionedScalar j1(const dimensionedScalar&);
dimensionedScalar jn(const int, const dimensionedScalar&);
dimensionedScalar y0(const dimensionedScalar&);
dimensionedScalar y1(const dimensionedScalar&);
dimensionedScalar yn(const int, const dimensionedScalar&);
}  // namespace mousse
#endif
