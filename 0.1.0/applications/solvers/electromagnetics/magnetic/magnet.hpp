// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::magnet
// Description
//   Class to hold the defining data for a permanent magnet, in particular
//   the name, relative permeability and remanence.
#ifndef magnet_hpp_
#define magnet_hpp_

#include "dimensioned_vector.hpp"

namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class magnet;
Istream& operator>>(Istream&, magnet&);
Ostream& operator<<(Ostream&, const magnet&);
class magnet
{
  // Private data
    word name_;
    scalar relativePermeability_;
    dimensionedScalar remanence_;
    vector orientation_;
public:
  // Constructors
    //- Null constructor for lists
    inline magnet()
    :
      remanence_{"Mr", dimensionSet(0, -1, 0, 0, 0, 1, 0), 0},
      orientation_{vector::zero}
    {}
    //- Construct from components
    inline magnet
    (
      const word& name,
      const scalar mur,
      const scalar Mr,
      const vector& orientation
    )
    :
      name_{name},
      relativePermeability_{mur},
      remanence_{"Mr", dimensionSet(0, -1, 0, 0, 0, 1, 0), Mr},
      orientation_{orientation}
    {}
    //- Construct from Istream
    inline magnet(Istream& is)
    :
      remanence_{"Mr", dimensionSet(0, -1, 0, 0, 0, 1, 0), 0},
      orientation_{vector::zero}
    {
      is >> *this;
    }
  // Member Functions
    //- Return name
    inline const word& name() const
    {
      return name_;
    }
    //- Return relative permeability
    inline scalar mur() const
    {
      return relativePermeability_;
    }
    //- Return remenance
    inline const dimensionedScalar& Mr() const
    {
      return remanence_;
    }
    //- Return orientation
    inline const vector& orientation() const
    {
      return orientation_;
    }
  // IOstream operators
    inline friend Istream& operator>>(Istream& is, magnet& m)
    {
      is.readBegin("magnet");
      is >> m.name_
        >> m.relativePermeability_
        >> m.remanence_.value()
        >> m.orientation_;
      is.readEnd("magnet");
      // Check state of Istream
      is.check("operator>>(Istream&, magnet&)");
      return is;
    }
    inline friend Ostream& operator<<(Ostream& os, const magnet& m)
    {
      os << token::BEGIN_LIST
        << m.name_ << token::SPACE
        << m.relativePermeability_ << token::SPACE
        << m.remanence_.value()
        << m.orientation_
        << token::END_LIST;
      return os;
    }
};
}  // namespace mousse
#endif
