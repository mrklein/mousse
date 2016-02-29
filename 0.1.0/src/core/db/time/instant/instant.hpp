#ifndef CORE_DB_TIME_INSTANT_INSTANT_HPP_
#define CORE_DB_TIME_INSTANT_INSTANT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::instant
// Description
//   An instant of time. Contains the time value and name.
// SourceFiles
//   instant.cpp
#include "word.hpp"
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class instant;
// Friend Operators
bool operator==(const instant&, const instant&);
bool operator!=(const instant&, const instant&);
// IOstream Operators
Istream& operator>>(Istream&, instant&);
Ostream& operator<<(Ostream&, const instant&);
class instant
{
  // Private data
    scalar value_;
    word name_;
public:
  // Public classes
    //- Less function class used in sorting instants
    class less
    {
    public:
      bool operator()(const instant& a, const instant& b) const
      {
        return a.value() < b.value();
      }
    };
  // Static data members
    static const char* const typeName;
  // Constructors
    //- Construct null
    instant();
    //- Construct from components
    instant(const scalar, const word&);
    //- Construct from time value
    explicit instant(const scalar);
    //- Construct from word
    explicit instant(const word&);
  // Member Functions
    // Access
      //- Value (const access)
      scalar value() const
      {
        return value_;
      }
      //- Value (non-const access)
      scalar& value()
      {
        return value_;
      }
      //- Name (const access)
      const word& name() const
      {
        return name_;
      }
      //- Name (non-const access)
      word& name()
      {
        return name_;
      }
      //- Comparison used for instants to be equal
      bool equal(const scalar) const;
  // Friend Operators
    friend bool operator==(const instant&, const instant&);
    friend bool operator!=(const instant&, const instant&);
  // IOstream Operators
    friend Istream& operator>>(Istream&, instant&);
    friend Ostream& operator<<(Ostream&, const instant&);
};
}  // namespace mousse
#endif
