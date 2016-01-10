// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   uint64.cpp
//   uint64_io.cpp
#ifndef uint64_hpp_
#define uint64_hpp_
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
//- Return a word representation of an uint64
word name(const uint64_t);
// IOstream Operators 
uint64_t readUint64(Istream&);
bool read(const char*, uint64_t&);
Istream& operator>>(Istream&, uint64_t&);
Ostream& operator<<(Ostream&, const uint64_t);
//- Template specialization for pTraits<uint64_t>
template<>
class pTraits<uint64_t>
{
  uint64_t p_;
public:
  //- Component type
  typedef uint64_t cmptType;
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
    static const uint64_t zero;
    static const uint64_t one;
    static const uint64_t min;
    static const uint64_t max;
    static const uint64_t rootMax;
    static const uint64_t rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const uint64_t&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the uint64_t value
    operator uint64_t() const
    {
      return p_;
    }
    //- Access to the uint64_t value
    operator uint64_t&()
    {
      return p_;
    }
};
}  // namespace mousse
#endif
