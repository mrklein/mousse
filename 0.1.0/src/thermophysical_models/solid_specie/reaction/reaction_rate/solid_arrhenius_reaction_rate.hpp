#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_REACTION_REACTION_RATE_SOLID_ARRHENIUS_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_REACTION_REACTION_RATE_SOLID_ARRHENIUS_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidArrheniusReactionRate
// Description
//   Arrhenius reaction rate for solids
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class solidArrheniusReactionRate
{
  // Private data
    scalar A_;
    scalar Ta_;
    scalar Tcrit_;
public:
  // Constructors
    //- Construct from components
    inline solidArrheniusReactionRate
    (
      const scalar A,
      const scalar Ta,
      const scalar Tcrit
    );
    //- Construct from Istream
    inline solidArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline solidArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  //- Destructor
  virtual ~solidArrheniusReactionRate()
  {}
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Arrhenius";
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
      const solidArrheniusReactionRate&
    );
};
}  // namespace mousse

// Constructors 
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const scalar A,
  const scalar Ta,
  const scalar Tcrit
  //const scalar nReact
)
:
  A_{A},
  Ta_{Ta},
  Tcrit_{Tcrit}
{}
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_{readScalar(is.readBegin("solidArrheniusReaction(Istream&)"))},
  Ta_{readScalar(is)},
  Tcrit_{readScalar(is)}
{}
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_{readScalar(dict.lookup("A"))},
  Ta_{readScalar(dict.lookup("Ta"))},
  Tcrit_{readScalar(dict.lookup("Tcrit"))}
{}
// Member Functions 
inline mousse::scalar mousse::solidArrheniusReactionRate::operator()
(
  const scalar,
  const scalar T,
  const scalarField&
) const
{
  scalar ak = A_;
  if (T < Tcrit_)
  {
    ak *= 0.0;
  }
  else
  {
    ak *= exp(-Ta_/T);
  }
  return ak;
}
inline void mousse::solidArrheniusReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
  os.writeKeyword("Tcrit") << Tcrit_ << token::END_STATEMENT << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const solidArrheniusReactionRate& arr
)
{
  os  << token::BEGIN_LIST
    << arr.A_ << token::SPACE << arr.Ta_ << token::SPACE << arr.Tcrit_
    << token::END_LIST;
  return os;
}
#endif
