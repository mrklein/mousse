// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::powerSeriesReactionRate
// Description
//   Power series reaction rate.
#ifndef power_series_reaction_rate_hpp_
#define power_series_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
#include "fixed_list.hpp"
namespace mousse
{
class powerSeriesReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
    static const label nCoeff_ = 4;
    FixedList<scalar, nCoeff_> coeffs_;
public:
  // Constructors
    //- Construct from components
    inline powerSeriesReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const FixedList<scalar, nCoeff_> coeffs
    );
    //- Construct from Istream
    inline powerSeriesReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline powerSeriesReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "powerSeries";
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
      const powerSeriesReactionRate&
    );
};
}  // namespace mousse

// Constructors 
inline mousse::powerSeriesReactionRate::powerSeriesReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta,
  const FixedList<scalar, nCoeff_> coeffs
)
:
  A_{A},
  beta_{beta},
  Ta_{Ta},
  coeffs_{coeffs}
{}
inline mousse::powerSeriesReactionRate::powerSeriesReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_{readScalar(is.readBegin("powerSeriesReactionRate(Istream&)"))},
  beta_{readScalar(is)},
  Ta_{readScalar(is)},
  coeffs_{is}
{
  is.readEnd("powerSeriesReactionRate(Istream&)");
}
inline mousse::powerSeriesReactionRate::powerSeriesReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_{readScalar(dict.lookup("A"))},
  beta_{readScalar(dict.lookup("beta"))},
  Ta_{readScalar(dict.lookup("Ta"))},
  coeffs_{dict.lookup("coeffs")}
{}
// Member Functions 
inline mousse::scalar mousse::powerSeriesReactionRate::operator()
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
  FOR_ALL(coeffs_, n)
  {
    expArg += coeffs_[n]/pow(T, n + 1);
  }
  lta *= exp(expArg);
  return lta;
}
inline void mousse::powerSeriesReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
  os.writeKeyword("beta") << beta_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
  os.writeKeyword("coeffs") << coeffs_ << token::END_STATEMENT << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const powerSeriesReactionRate& psrr
)
{
  os << token::BEGIN_LIST;
  os << psrr.A_ << token::SPACE << psrr.beta_ << token::SPACE << psrr.Ta_;
  for (int n=0; n<powerSeriesReactionRate::nCoeff_; n++)
  {
    os << token::SPACE << psrr.coeffs_[n];
  }
  os << token::END_LIST;
  return os;
}
#endif
