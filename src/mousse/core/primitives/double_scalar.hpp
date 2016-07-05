#ifndef CORE_PRIMITIVES_SCALAR_DOUBLE_SCALAR_HPP_
#define CORE_PRIMITIVES_SCALAR_DOUBLE_SCALAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   double_scalar.cpp

#include "double_float.hpp"
#include "direction.hpp"
#include "word.hpp"


namespace mousse {

typedef double doubleScalar;

// Largest and smallest scalar values allowed in certain parts of the code.
// (15 is the number of significant figures in an
//  IEEE double precision number.  See limits.h or float.h)
static const doubleScalar doubleScalarGREAT = 1.0e+15;
static const doubleScalar doubleScalarVGREAT = 1.0e+300;
static const doubleScalar doubleScalarROOTVGREAT = 1.0e+150;
static const doubleScalar doubleScalarSMALL = 1.0e-15;
static const doubleScalar doubleScalarROOTSMALL = 3.0e-8;
static const doubleScalar doubleScalarVSMALL = 1.0e-300;
static const doubleScalar doubleScalarROOTVSMALL = 1.0e-150;


//- Read whole of buf as a scalar. Return true if succesful.
inline bool readScalar(const char* buf, doubleScalar& s)
{
  char* endPtr;
  s = strtod(buf, &endPtr);
  return (*endPtr == '\0');
}


#define Scalar doubleScalar
#define ScalarVGREAT doubleScalarVGREAT
#define ScalarVSMALL doubleScalarVSMALL
#define ScalarROOTVGREAT doubleScalarROOTVGREAT
#define ScalarROOTVSMALL doubleScalarROOTVSMALL
#define readScalar readDoubleScalar


inline Scalar mag(const Scalar s)
{
  return ::fabs(s);
}


#define transFunc(func)                                                       \
inline Scalar func(const Scalar s)                                            \
{                                                                             \
  return ::func(s);                                                           \
}

}  // namespace mousse


#include "scalar.inc"


namespace mousse {

inline Scalar hypot(const Scalar x, const Scalar y)
{
  return ::hypot(x, y);
}


inline Scalar atan2(const Scalar y, const Scalar x)
{
  return ::atan2(y, x);
}


inline Scalar jn(const int n, const Scalar s)
{
  return ::jn(n, s);
}


inline Scalar yn(const int n, const Scalar s)
{
  return ::yn(n, s);
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
