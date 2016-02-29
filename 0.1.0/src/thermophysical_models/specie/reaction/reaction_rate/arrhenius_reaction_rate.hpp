#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_ARRHENIUS_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_ARRHENIUS_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ArrheniusReactionRate
// Description
//   Arrhenius reaction rate given by:
//     k = A * T^beta * exp(-Ta/T)
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class ArrheniusReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
public:
  // Constructors
    //- Construct from components
    inline ArrheniusReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta
    );
    //- Construct from Istream
    inline ArrheniusReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline ArrheniusReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
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
      const ArrheniusReactionRate&
    );
};
}  // namespace mousse

// Constructors 
inline mousse::ArrheniusReactionRate::ArrheniusReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta
)
:
  A_{A},
  beta_{beta},
  Ta_{Ta}
{}
inline mousse::ArrheniusReactionRate::ArrheniusReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_{readScalar(is.readBegin("ArrheniusReactionRate(Istream&)"))},
  beta_{readScalar(is)},
  Ta_{readScalar(is)}
{
  is.readEnd("ArrheniusReactionRate(Istream&)");
}
inline mousse::ArrheniusReactionRate::ArrheniusReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_{readScalar(dict.lookup("A"))},
  beta_{readScalar(dict.lookup("beta"))},
  Ta_{readScalar(dict.lookup("Ta"))}
{}
// Member Functions 
inline mousse::scalar mousse::ArrheniusReactionRate::operator()
(
  const scalar /*p*/,
  const scalar T,
  const scalarField&
) const
{
  scalar ak = A_;
  if (mag(beta_) > VSMALL)
  {
    ak *= pow(T, beta_);
  }
  if (mag(Ta_) > VSMALL)
  {
    ak *= exp(-Ta_/T);
  }
  return ak;
}
inline void mousse::ArrheniusReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
  os.writeKeyword("beta") << beta_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const ArrheniusReactionRate& arr
)
{
  os<< token::BEGIN_LIST
    << arr.A_ << token::SPACE << arr.beta_ << token::SPACE << arr.Ta_
    << token::END_LIST;
  return os;
}
#endif
