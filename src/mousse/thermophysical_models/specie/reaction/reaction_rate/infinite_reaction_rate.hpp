#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_INFINITE_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_INFINITE_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::infiniteReactionRate
// Description
//   infinite reaction rate.

#include "scalar_field.hpp"
#include "type_info.hpp"


namespace mousse {

class infiniteReactionRate
{
public:
  // Constructors
    //- Null constructor
    inline infiniteReactionRate
    ();
    //- Construct from Istream
    inline infiniteReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "infinite";
    }
    inline scalar operator()
    (
      const scalar p,
      const scalar T,
      const scalarField& c
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    inline friend Ostream& operator<<
    (
      Ostream&,
      const infiniteReactionRate&
    );
};

}  // namespace mousse

// Constructors 
inline mousse::infiniteReactionRate::infiniteReactionRate()
{}


inline mousse::infiniteReactionRate::infiniteReactionRate
(
  const speciesTable&,
  const dictionary&
)
{}


inline void mousse::infiniteReactionRate::write(Ostream&) const
{}


// Member Functions 
inline mousse::scalar mousse::infiniteReactionRate::operator()
(
  const scalar /*p*/,
  const scalar,
  const scalarField&
) const
{
  return 1.0;
}


inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const infiniteReactionRate& /*rr*/
)
{
  os << token::BEGIN_LIST
    << token::END_LIST;
  return os;
}

#endif

