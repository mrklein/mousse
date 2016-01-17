// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LandauTellerReactionRate
// Description
//   Landau-Teller reaction rate.
#ifndef landau_teller_reaction_rate_hpp_
#define landau_teller_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class LandauTellerReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
    scalar B_;
    scalar C_;
public:
  // Constructors
    //- Construct from components
    inline LandauTellerReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const scalar B,
      const scalar C
    );
    //- Construct from Istream
    inline LandauTellerReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline LandauTellerReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "LandauTeller";
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
      const LandauTellerReactionRate&
    );
};
}  // namespace mousse

// Constructors 
inline mousse::LandauTellerReactionRate::LandauTellerReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta,
  const scalar B,
  const scalar C
)
:
  A_{A},
  beta_{beta},
  Ta_{Ta},
  B_{B},
  C_{C}
{}
inline mousse::LandauTellerReactionRate::LandauTellerReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_{readScalar(is.readBegin("LandauTellerReactionRate(Istream&)"))},
  beta_{readScalar(is)},
  Ta_{readScalar(is)},
  B_{readScalar(is)},
  C_{readScalar(is)}
{
  is.readEnd("LandauTellerReactionRate(Istream&)");
}
inline mousse::LandauTellerReactionRate::LandauTellerReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_{readScalar(dict.lookup("A"))},
  beta_{readScalar(dict.lookup("beta"))},
  Ta_{readScalar(dict.lookup("Ta"))},
  B_{readScalar(dict.lookup("B"))},
  C_{readScalar(dict.lookup("C"))}
{}
// Member Functions 
inline mousse::scalar mousse::LandauTellerReactionRate::operator()
(
  const scalar /*p*/,
  const scalar T,
  const scalarField&
) const
{
  scalar lta = A_;
  if (mag(beta_) > VSMALL)
  {
    lta *= pow(T, beta_);
  }
  scalar expArg = 0.0;
  if (mag(Ta_) > VSMALL)
  {
    expArg -= Ta_/T;
  }
  if (mag(B_) > VSMALL)
  {
    expArg += B_/cbrt(T);
  }
  if (mag(C_) > VSMALL)
  {
    expArg += C_/pow(T, 2.0/3.0);
  }
  if (mag(expArg) > VSMALL)
  {
    lta *= exp(expArg);
  }
  return lta;
}
inline void mousse::LandauTellerReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
  os.writeKeyword("beta") << beta_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
  os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
  os.writeKeyword("C") << C_ << token::END_STATEMENT << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const LandauTellerReactionRate& arr
)
{
  os<< token::BEGIN_LIST
    << arr.A_ << token::SPACE << arr.beta_ << token::SPACE << arr.Ta_
    << token::SPACE << arr.B_ << token::SPACE << arr.C_
    << token::END_LIST;
  return os;
}
#endif
