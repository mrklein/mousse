#ifndef CORE_PRIMITIVES_INTS_INT_HPP_
#define CORE_PRIMITIVES_INTS_INT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "int32.hpp"
#include "int64.hpp"
#if defined(darwin64) && defined(__clang__)
#include "long.hpp"
#include "ulong.hpp"
#endif


namespace mousse {

#define MAXMIN(retType, type1, type2)              \
                         \
inline retType max(const type1 s1, const type2 s2) \
{                                                  \
  return (s1 > s2)? s1: s2;                      \
}                                                  \
                         \
inline retType min(const type1 s1, const type2 s2) \
{                                                  \
  return (s1 < s2)? s1: s2;                      \
}


MAXMIN(int8_t, int8_t, int8_t)
MAXMIN(int16_t, int16_t, int16_t)
MAXMIN(int32_t, int32_t, int32_t)
MAXMIN(int64_t, int64_t, int32_t)
MAXMIN(int64_t, int32_t, int64_t)
MAXMIN(int64_t, int64_t, int64_t)
#if defined(darwin64) && defined(__clang__)
MAXMIN(long, long, long)
MAXMIN(unsigned long, unsigned long, unsigned long)
#endif

// IOstream Operators 
int readInt(Istream&);

}  // namespace mousse
#endif
