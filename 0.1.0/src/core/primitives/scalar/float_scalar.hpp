#ifndef CORE_PRIMITIVES_SCALAR_FLOAT_SCALAR_HPP_
#define CORE_PRIMITIVES_SCALAR_FLOAT_SCALAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   float_scalar.cpp
#include "double_float.hpp"
#include "direction.hpp"
#include "word.hpp"
namespace mousse
{
typedef float floatScalar;
// Largest and smallest scalar values allowed in certain parts of the code
// (6 is the number of significant figures in an
//  IEEE single precision number.  See limits.h or float.h)
static const floatScalar floatScalarGREAT = 1.0e+6;
static const floatScalar floatScalarVGREAT = 1.0e+37;
static const floatScalar floatScalarROOTVGREAT = 1.0e+18;
static const floatScalar floatScalarSMALL = 1.0e-6;
static const floatScalar floatScalarROOTSMALL = 1.0e-3;
static const floatScalar floatScalarVSMALL = 1.0e-37;
static const floatScalar floatScalarROOTVSMALL = 1.0e-18;
//- Read whole of buf as a scalar. Return true if succesful.
inline bool readScalar(const char* buf, floatScalar& s)
{
  char* endPtr;
  s = strtof(buf, &endPtr);
  return (*endPtr == '\0');
}
#define Scalar floatScalar
#define ScalarVGREAT floatScalarVGREAT
#define ScalarVSMALL floatScalarVSMALL
#define ScalarROOTVGREAT floatScalarROOTVGREAT
#define ScalarROOTVSMALL floatScalarROOTVSMALL
#define readScalar readFloatScalar
inline Scalar mag(const Scalar s)
{
  return ::fabsf(s);
}
#define transFunc(func)            \
inline Scalar func(const Scalar s) \
{                                  \
  return ::func##f(s);           \
}
}  // namespace mousse
#include "_scalar.hpp"
namespace mousse
{
inline Scalar hypot(const Scalar x, const Scalar y)
{
  return ::hypotf(x, y);
}
inline Scalar atan2(const Scalar y, const Scalar x)
{
  return ::atan2f(y, x);
}
inline Scalar jn(const int n, const Scalar s)
{
  return ::jnf(n, s);
}
inline Scalar yn(const int n, const Scalar s)
{
  return ::ynf(n, s);
}
#undef Scalar
#undef ScalarVGREAT
#undef ScalarVSMALL
#undef ScalarROOTVGREAT
#undef ScalarROOTVSMALL
#undef readScalar
#undef transFunc
}  // namespace mousse
#endif
