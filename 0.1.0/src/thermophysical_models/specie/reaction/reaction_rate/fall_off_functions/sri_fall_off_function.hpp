#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_SRI_FALL_OFF_FUNCTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_SRI_FALL_OFF_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRIFallOffFunction
// Description
//   The SRI fall-off function

#include "scalar.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class SRIFallOffFunction;
Ostream& operator<<(Ostream&, const SRIFallOffFunction&);


class SRIFallOffFunction
{
  // Private data
    scalar a_, b_, c_, d_, e_;
public:
  // Constructors
    //- Construct from components
    inline SRIFallOffFunction
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e
    );
    //- Construct from Istream
    inline SRIFallOffFunction(Istream&);
    //- Construct from dictionary
    inline SRIFallOffFunction(const dictionary& dict);
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "SRI";
    }
    inline scalar operator()
    (
      const scalar T,
      const scalar Pr
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const SRIFallOffFunction&);
};

}  // namespace mousse


// Constructors 
inline mousse::SRIFallOffFunction::SRIFallOffFunction
(
  const scalar a,
  const scalar b,
  const scalar c,
  const scalar d,
  const scalar e
)
:
  a_{a},
  b_{b},
  c_{c},
  d_{d},
  e_{e}
{}


inline mousse::SRIFallOffFunction::SRIFallOffFunction(Istream& is)
:
  a_{readScalar(is.readBegin("SRIFallOffFunction(Istream&)"))},
  b_{readScalar(is)},
  c_{readScalar(is)},
  d_{readScalar(is)},
  e_{readScalar(is)}
{
  is.readEnd("SRIFallOffFunction(Istream&)");
}


inline mousse::SRIFallOffFunction::SRIFallOffFunction(const dictionary& dict)
:
  a_{readScalar(dict.lookup("a"))},
  b_{readScalar(dict.lookup("b"))},
  c_{readScalar(dict.lookup("c"))},
  d_{readScalar(dict.lookup("d"))},
  e_{readScalar(dict.lookup("e"))}
{}


// Member Functions 
inline mousse::scalar mousse::SRIFallOffFunction::operator()
(
  const scalar T,
  const scalar Pr
) const
{
  scalar X = 1.0/(1.0 + sqr(log10(max(Pr, SMALL))));
  return d_*pow(a_*exp(-b_/T) + exp(-T/c_), X)*pow(T, e_);
}


inline void mousse::SRIFallOffFunction::write(Ostream& os) const
{
  os.writeKeyword("a") << a_ << token::END_STATEMENT << nl;
  os.writeKeyword("b") << b_ << token::END_STATEMENT << nl;
  os.writeKeyword("c") << c_ << token::END_STATEMENT << nl;
  os.writeKeyword("d") << d_ << token::END_STATEMENT << nl;
  os.writeKeyword("e") << e_ << token::END_STATEMENT << nl;
}


// Ostream Operator 
inline mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::SRIFallOffFunction& srifof
)
{
  os << token::BEGIN_LIST
    << srifof.a_
    << token::SPACE << srifof.b_
    << token::SPACE << srifof.c_
    << token::SPACE << srifof.d_
    << token::SPACE << srifof.e_
    << token::END_LIST;
  return os;
}

#endif
