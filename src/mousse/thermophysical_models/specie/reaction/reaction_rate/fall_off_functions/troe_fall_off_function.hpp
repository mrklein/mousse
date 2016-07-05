#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_TROE_FALL_OFF_FUNCTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_TROE_FALL_OFF_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TroeFallOffFunction
// Description
//   The Troe fall-off function

#include "scalar.hpp"


namespace mousse {
// Forward declaration of friend functions and operators
class TroeFallOffFunction;
Ostream& operator<<(Ostream&, const TroeFallOffFunction&);


class TroeFallOffFunction
{
  // Private data
    scalar alpha_;
    scalar Tsss_, Ts_, Tss_;
public:
  // Constructors
    //- Construct from components
    inline TroeFallOffFunction
    (
      const scalar alpha,
      const scalar Tsss,
      const scalar Ts,
      const scalar Tss
    );
    //- Construct from Istream
    inline TroeFallOffFunction(Istream&);
    //- Construct from dictionary
    inline TroeFallOffFunction(const dictionary& dict);
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Troe";
    }
    inline scalar operator()
    (
      const scalar T,
      const scalar Pr
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const TroeFallOffFunction&);
};

}  // namespace mousse


// Constructors 
inline mousse::TroeFallOffFunction::TroeFallOffFunction
(
  const scalar alpha,
  const scalar Tsss,
  const scalar Ts,
  const scalar Tss
)
:
  alpha_{alpha},
  Tsss_{Tsss},
  Ts_{Ts},
  Tss_{Tss}
{}


inline mousse::TroeFallOffFunction::TroeFallOffFunction(Istream& is)
:
  alpha_{readScalar(is.readBegin("TroeFallOffFunction(Istream&)"))},
  Tsss_{readScalar(is)},
  Ts_{readScalar(is)},
  Tss_{readScalar(is)}
{
  is.readEnd("TroeFallOffFunction(Istream&)");
}


inline mousse::TroeFallOffFunction::TroeFallOffFunction(const dictionary& dict)
:
  alpha_{readScalar(dict.lookup("alpha"))},
  Tsss_{readScalar(dict.lookup("Tsss"))},
  Ts_{readScalar(dict.lookup("Ts"))},
  Tss_{readScalar(dict.lookup("Tss"))}
{}


// Member Functions 
inline mousse::scalar mousse::TroeFallOffFunction::operator()
(
  const scalar T,
  const scalar Pr
) const
{
  scalar logFcent =
    log10
    (
      max((1 - alpha_)*exp(-T/Tsss_) + alpha_*exp(-T/Ts_) + exp(-Tss_/T),
          SMALL)
    );
  scalar c = -0.4 - 0.67*logFcent;
  static const scalar d = 0.14;
  scalar n = 0.75 - 1.27*logFcent;
  scalar logPr = log10(max(Pr, SMALL));
  return pow(10.0, logFcent/(1.0 + sqr((logPr + c)/(n - d*(logPr + c)))));
}


inline void mousse::TroeFallOffFunction::write(Ostream& os) const
{
  os.writeKeyword("alpha") << alpha_ << token::END_STATEMENT << nl;
  os.writeKeyword("Tsss") << Tsss_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ts") << Ts_ << token::END_STATEMENT << nl;
  os.writeKeyword("Tss") << Tss_ << token::END_STATEMENT << nl;
}


// Ostream Operator 
inline mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::TroeFallOffFunction& tfof
)
{
  os << token::BEGIN_LIST
    << tfof.alpha_
    << token::SPACE << tfof.Tsss_
    << token::SPACE << tfof.Ts_
    << token::SPACE << tfof.Tss_
    << token::END_LIST;
  return os;
}

#endif
