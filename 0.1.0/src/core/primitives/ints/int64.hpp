#ifndef CORE_PRIMITIVES_INTS_INT64_HPP_
#define CORE_PRIMITIVES_INTS_INT64_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   int64.cpp
//   int64_io.cpp
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <climits>
#include <cstdlib>
#include "word.hpp"
#include "ptraits.hpp"
#include "direction.hpp"
#ifndef UINT64_MIN
#define UINT64_MIN 0
#endif
namespace mousse
{
class Istream;
class Ostream;
//- Return a word representation of an int64
word name(const int64_t);
// IOstream Operators 
int64_t readInt64(Istream&);
bool read(const char*, int64_t&);
Istream& operator>>(Istream&, int64_t&);
Ostream& operator<<(Ostream&, const int64_t);
//- Template specialization for pTraits<int64_t>
template<>
class pTraits<int64_t>
{
  int64_t p_;
public:
  //- Component type
  typedef int64_t cmptType;
  // Member constants
    enum
    {
      dim = 3,         //!< Dimensionality of space
      rank = 0,        //!< Rank of int64_t is 0
      nComponents = 1  //!< Number of components in int64_t is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const int64_t zero;
    static const int64_t one;
    static const int64_t min;
    static const int64_t max;
    static const int64_t rootMax;
    static const int64_t rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const int64_t&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the int64_t value
    operator int64_t() const
    {
      return p_;
    }
    //- Access to the int value
    operator int64_t&()
    {
      return p_;
    }
};
inline int64_t mag(const int64_t l)
{
  return ::labs(l);
}
}  // namespace mousse
#endif
