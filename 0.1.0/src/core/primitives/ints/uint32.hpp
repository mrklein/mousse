// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   uint32.cpp
//   uint32_io.cpp
#ifndef uint32_hpp_
#define uint32_hpp_
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
namespace mousse
{
class Istream;
class Ostream;
//- Return a word representation of an uint32
word name(const uint32_t);
// IOstream Operators 
uint32_t readUint32(Istream&);
bool read(const char*, uint32_t&);
Istream& operator>>(Istream&, uint32_t&);
Ostream& operator<<(Ostream&, const uint32_t);
//- Template specialization for pTraits<uint32_t>
template<>
class pTraits<uint32_t>
{
  uint32_t p_;
public:
  //- Component type
  typedef uint32_t cmptType;
  // Member constants
    enum
    {
      dim = 3,         //!< Dimensionality of space
      rank = 0,        //!< Rank of uint32_t is 0
      nComponents = 1  //!< Number of components in uint32_t is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const uint32_t zero;
    static const uint32_t one;
    static const uint32_t min;
    static const uint32_t max;
    static const uint32_t rootMax;
    static const uint32_t rootMin;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const uint32_t&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the uint32_t value
    operator uint32_t() const
    {
      return p_;
    }
    //- Access to the uint32_t value
    operator uint32_t&()
    {
      return p_;
    }
};
}  // namespace mousse
#endif