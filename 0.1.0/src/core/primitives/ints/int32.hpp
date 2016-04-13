#ifndef CORE_PRIMITIVES_INTS_INT32_HPP_
#define CORE_PRIMITIVES_INTS_INT32_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <climits>
#include <cstdlib>
#include "word.hpp"
#include "ptraits.hpp"
#include "direction.hpp"
#ifndef UINT32_MIN
#define UINT32_MIN 0
#endif


namespace mousse {

class Istream;
class Ostream;

//- Return a word representation of an int32
word name(const int32_t);

// IOstream Operators 
int32_t readInt32(Istream&);
bool read(const char*, int32_t&);
Istream& operator>>(Istream&, int32_t&);
Ostream& operator<<(Ostream&, const int32_t);

// On 32bit OSs long is not unambiguously int32_t (or int64_t) causing problems
// for IO operator resolution.
// This problem is avoided by explicitly defining the following operators:
#if WM_ARCH_OPTION == 32
  Istream& operator>>(Istream&, long&);
  Ostream& operator<<(Ostream&, const long);
#endif

//- Template specialization for pTraits<int32_t>
template<>
class pTraits<int32_t>
{
  int32_t p_;
public:
  //- Component type
  typedef int32_t cmptType;
  // Member constants
    enum
    {
      dim = 3,         //!< Dimensionality of space
      rank = 0,        //!< Rank of int32_t is 0
      nComponents = 1  //!< Number of components in int32_t is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const int32_t zero;
    static const int32_t one;
    static const int32_t min;
    static const int32_t max;
    static const int32_t rootMax;
    static const int32_t rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const int32_t&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the int32_t value
    operator int32_t() const
    {
      return p_;
    }
    //- Access to the int value
    operator int32_t&()
    {
      return p_;
    }
};

inline int32_t mag(const int32_t l)
{
  return ::abs(l);
}

}  // namespace mousse

#endif
