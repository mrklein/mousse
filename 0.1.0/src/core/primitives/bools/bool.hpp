// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   bool_io.cpp
#ifndef bool_hpp_
#define bool_hpp_
namespace mousse
{
class Istream;
class Ostream;
Istream& operator>>(Istream&, bool&);
Ostream& operator<<(Ostream&, const bool);
bool readBool(Istream&);
}  // namespace mousse
#include "ptraits.hpp"
namespace mousse
{
// template specialisation for pTraits<bool>
template<>
class pTraits<bool>
{
  bool p_;
public:
  //- Component type
  typedef bool cmptType;
  // Member constants
    enum
    {
      dim = 3,         //!< Dimensionality of space
      rank = 0,        //!< Rank of bool is 0
      nComponents = 1  //!< Number of components in bool is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const bool zero;
    static const bool one;
  // Constructors
    //- Construct from primitive
    explicit pTraits(const bool&);
    //- Construct from Istream
    pTraits(Istream&);
  // Member Functions
    //- Access to the bool value
    operator bool() const
    {
      return p_;
    }
    //- Access to the bool value
    operator bool&()
    {
      return p_;
    }
};
}  // namespace mousse
#endif
