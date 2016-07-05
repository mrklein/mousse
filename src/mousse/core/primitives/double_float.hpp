#ifndef CORE_PRIMITIVES_SCALAR_DOUBLE_FLOAT_HPP_
#define CORE_PRIMITIVES_SCALAR_DOUBLE_FLOAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label.hpp"
#include "products.hpp"
#include <cmath>


#if defined(darwin64)
inline float j0f(float x) { return float(j0(double(x)));}
inline float j1f(float x) { return float(j1(double(x)));}
inline float y0f(float x) { return float(y0(double(x)));}
inline float y1f(float x) { return float(y1(double(x)));}
inline float jnf(const int n, const float s) { return float(jn(n, double(s))); }
inline float ynf(const int n, const float s) { return float(yn(n, double(s))); }
#endif

namespace mousse {

template<class Cmpt>
class typeOfRank<Cmpt, 0>
{
public:
  typedef Cmpt type;
};


template<class Cmpt>
class symmTypeOfRank<Cmpt, 0>
{
public:
  typedef Cmpt type;
};


template<class T>
inline bool equal(const T& s1, const T& s2)
{
  return s1 == s2;
}

#define MAXMINPOW(retType, type1, type2)                                      \
                                                                              \
MAXMIN(retType, type1, type2)                                                 \
                                                                              \
inline double pow(const type1 s, const type2 e)                               \
{                                                                             \
  return ::pow(double(s), double(e));                                         \
}

MAXMINPOW(double, double, double)
MAXMINPOW(double, double, float)
MAXMINPOW(double, float, double)
MAXMINPOW(float, float, float)
MAXMINPOW(double, double, int)
MAXMINPOW(double, int, double)
MAXMINPOW(double, double, long)
MAXMINPOW(double, long, double)
MAXMINPOW(float, float, int)
MAXMINPOW(float, int, float)
MAXMINPOW(float, float, long)
MAXMINPOW(float, long, float)

#if defined(darwin64) && defined(__clang__) && WM_LABEL_SIZE == 64
MAXMINPOW(double, int64_t, double)
MAXMINPOW(float, int64_t, float)
MAXMINPOW(double, double, int64_t)
MAXMINPOW(float, float, int64_t)
#endif

#undef MAXMINPOW
}  // namespace mousse
#endif
