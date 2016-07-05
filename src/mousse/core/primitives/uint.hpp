#ifndef CORE_PRIMITIVES_INTS_UINT_HPP_
#define CORE_PRIMITIVES_INTS_UINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uint32.hpp"
#include "uint64.hpp"


namespace mousse {

#define MAXMIN(retType, type1, type2)                                         \
                                                                              \
inline retType max(const type1 s1, const type2 s2)                            \
{                                                                             \
  return (s1 > s2)? s1: s2;                                                   \
}                                                                             \
                                                                              \
inline retType min(const type1 s1, const type2 s2)                            \
{                                                                             \
  return (s1 < s2)? s1: s2;                                                   \
}


MAXMIN(uint8_t, uint8_t, uint8_t)
MAXMIN(uint16_t, uint16_t, uint16_t)
MAXMIN(uint32_t, uint32_t, uint32_t)
MAXMIN(uint64_t, uint64_t, uint32_t)
MAXMIN(uint64_t, uint32_t, uint64_t)
MAXMIN(uint64_t, uint64_t, uint64_t)

// IOstream Operators
uint readUint(Istream&);

}  // namespace mousse

#endif
