#ifndef CORE_PRIMITIVES_INTS_ULONG_HPP_
#define CORE_PRIMITIVES_INTS_ULONG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ulong.cpp
//   ulong_io.cpp
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <climits>
#include <cstdlib>
#include "word.hpp"
#include "ptraits.hpp"
#include "direction.hpp"
#ifndef ULONG_MIN
#define ULONG_MIN 0
#endif
namespace mousse
{
class Istream;
class Ostream;
//- Return a word representation of an unsigned long
word name(const unsigned long);
// IOstream Operators 
unsigned long readUlong(Istream&);
bool read(const char*, unsigned long&);
Istream& operator>>(Istream&, unsigned long&);
Ostream& operator<<(Ostream&, const unsigned long);
//- Template specialization for pTraits<unsigned long>
template<>
class pTraits<unsigned long>
{
  unsigned long p_;
public:
  //- Component type
  typedef unsigned long cmptType;
  // Member constants
    enum
    {
      dim = 3,         //!< Dimensionality of space
      rank = 0,        //!< Rank of uint64_t is 0
      nComponents = 1  //!< Number of components in uint64_t is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const unsigned long zero;
    static const unsigned long one;
    static const unsigned long min;
    static const unsigned long max;
    static const unsigned long rootMax;
    static const unsigned long rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const unsigned long&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the uint64_t value
    operator unsigned long() const
    {
      return p_;
    }
    //- Access to the uint value
    operator unsigned long&()
    {
      return p_;
    }
};
}  // namespace mousse
#endif
