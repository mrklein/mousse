#ifndef CORE_PRIMITIVES_INTS_LONG_HPP_
#define CORE_PRIMITIVES_INTS_LONG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   long.cpp
//   long_io.cpp
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <climits>
#include <cstdlib>
#include "word.hpp"
#include "ptraits.hpp"
#include "direction.hpp"
#ifndef LONG_MIN
#define LONG_MIN 0
#endif
namespace mousse
{
class Istream;
class Ostream;
//- Return a word representation of an long
word name(const long);
// IOstream Operators 
long readLong(Istream&);
bool read(const char*, long&);
Istream& operator>>(Istream&, long&);
Ostream& operator<<(Ostream&, const long);
//- Template specialization for pTraits<long>
template<>
class pTraits<long>
{
  long p_;
public:
  //- Component type
  typedef long cmptType;
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
    static const long zero;
    static const long one;
    static const long min;
    static const long max;
    static const long rootMax;
    static const long rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const long&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the long value
    operator long() const
    {
      return p_;
    }
    //- Access to the long value
    operator long&()
    {
      return p_;
    }
};
}  // namespace mousse
#endif
