// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::substance
// Description
// SourceFiles
//   substance_i.hpp
//   substance.cpp
//   substance_io.cpp
#ifndef substance_hpp_
#define substance_hpp_
#include "scalar.hpp"
#include "word.hpp"
#include "istream.hpp"
namespace mousse
{
class substance
{
  // Private data
    word name_;
    scalar volFrac_;
public:
  // Constructors
    //- Construct null
    substance()
    {}
  // Member Functions
    // Access
      const word& name() const
      {
        return name_;
      }
      scalar volFrac() const
      {
        return volFrac_;
      }
      bool operator==(const substance& s) const
      {
        return name_ == s.name_ && volFrac_ == s.volFrac_;
      }
      bool operator!=(const substance& s) const
      {
        return !operator==(s);
      }
  // IOstream Operators
    friend Istream& operator>>(Istream& is, substance& s)
    {
      is  >> s.name_ >> s.volFrac_;
      return is;
    }
    friend Ostream& operator<<(Ostream& os, const substance& s)
    {
      os  << s.name_ << token::SPACE << s.volFrac_;
      return os;
    }
};
}  // namespace mousse
#endif
