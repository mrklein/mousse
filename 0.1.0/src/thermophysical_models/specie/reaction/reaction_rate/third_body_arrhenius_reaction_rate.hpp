#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_THIRD_BODY_ARRHENIUS_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_THIRD_BODY_ARRHENIUS_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thirdBodyArrheniusReactionRate
// Description
//   Arrhenius reaction rate enhanced by third-body interation.
#include "arrhenius_reaction_rate.hpp"
#include "third_body_efficiencies.hpp"
namespace mousse
{
class thirdBodyArrheniusReactionRate
:
  ArrheniusReactionRate
{
  // Private data
    thirdBodyEfficiencies thirdBodyEfficiencies_;
public:
  // Constructors
    //- Construct from components
    inline thirdBodyArrheniusReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const thirdBodyEfficiencies& tbes
    );
    //- Construct from Istream
    inline thirdBodyArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline thirdBodyArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "thirdBodyArrhenius";
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
      const thirdBodyArrheniusReactionRate&
    );
};
}  // namespace mousse

// Constructors 
inline mousse::thirdBodyArrheniusReactionRate::thirdBodyArrheniusReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta,
  const thirdBodyEfficiencies& tbes
)
:
  ArrheniusReactionRate{A, beta, Ta},
  thirdBodyEfficiencies_{tbes}
{}
inline mousse::thirdBodyArrheniusReactionRate::thirdBodyArrheniusReactionRate
(
  const speciesTable& species,
  Istream& is
)
:
  ArrheniusReactionRate
  {
    species,
    is.readBegin("thirdBodyArrheniusReactionRate(Istream&)")
  },
  thirdBodyEfficiencies_{species, is}
{
  is.readEnd("thirdBodyArrheniusReactionRate(Istream&)");
}
inline mousse::thirdBodyArrheniusReactionRate::thirdBodyArrheniusReactionRate
(
  const speciesTable& species,
  const dictionary& dict
)
:
  ArrheniusReactionRate
  {
    species,
    dict
  },
  thirdBodyEfficiencies_{species, dict}
{}
// Member Functions 
inline mousse::scalar mousse::thirdBodyArrheniusReactionRate::operator()
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  return thirdBodyEfficiencies_.M(c)
    *ArrheniusReactionRate::operator()(p, T, c);
}
inline void mousse::thirdBodyArrheniusReactionRate::write(Ostream& os) const
{
  ArrheniusReactionRate::write(os);
  thirdBodyEfficiencies_.write(os);
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const thirdBodyArrheniusReactionRate& arr
)
{
  os << token::BEGIN_LIST
    << static_cast<const ArrheniusReactionRate&>(arr)
    << token::SPACE << arr.thirdBodyEfficiencies_
    << token::END_LIST;
  return os;
}
#endif
