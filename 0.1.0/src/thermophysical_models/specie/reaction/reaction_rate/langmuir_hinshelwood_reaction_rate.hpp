#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_LANGMUIR_HINSHELWOOD_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_LANGMUIR_HINSHELWOOD_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LangmuirHinshelwoodReactionRate
// Description
//   Power series reaction rate.

#include "scalar_field.hpp"
#include "type_info.hpp"
#include "fixed_list.hpp"
#include "tuple2.hpp"


namespace mousse {

class LangmuirHinshelwoodReactionRate
{
  // Private data
    static const label n_ = 5;
    scalar A_[n_];
    scalar Ta_[n_];
    label co_;
    label c3h6_;
    label no_;
public:
  // Constructors
    //- Construct from components
    inline LangmuirHinshelwoodReactionRate
    (
      const scalar A[],
      const scalar Ta[],
      const label co,
      const label c3h6,
      const label no
    );
    //- Construct from Istream
    inline LangmuirHinshelwoodReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline LangmuirHinshelwoodReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "LangmuirHinshelwood";
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
      const LangmuirHinshelwoodReactionRate&
    );
};

}  // namespace mousse


// Constructors 
inline mousse::LangmuirHinshelwoodReactionRate::LangmuirHinshelwoodReactionRate
(
  const scalar A[],
  const scalar Ta[],
  const label co,
  const label c3h6,
  const label no
)
:
  co_{co},
  c3h6_{c3h6},
  no_{no}
{
  for (int i=0; i<n_; i++) {
    A_[i] = A[i];
    Ta_[i] = Ta[i];
  }
}


inline mousse::LangmuirHinshelwoodReactionRate::LangmuirHinshelwoodReactionRate
(
  const speciesTable& st,
  Istream& is
)
:
  co_{st["CO"]},
  c3h6_{st["C3H6"]},
  no_{st["NO"]}
{
  is.readBegin("LangmuirHinshelwoodReactionRate(Istream&)");
  for (int i=0; i<n_; i++) {
    is >> A_[i] >> Ta_[i];
  }
  is.readEnd("LangmuirHinshelwoodReactionRate(Istream&)");
}


inline mousse::LangmuirHinshelwoodReactionRate::LangmuirHinshelwoodReactionRate
(
  const speciesTable& st,
  const dictionary& dict
)
:
  co_{st["CO"]},
  c3h6_{st["C3H6"]},
  no_{st["NO"]}
{
  // read (A, Ta) pairs
  FixedList<Tuple2<scalar, scalar>, n_> coeffs{dict.lookup("coeffs")};
  FOR_ALL(coeffs, i) {
    A_[i] = coeffs[i].first();
    Ta_[i] = coeffs[i].second();
  }
}


// Member Functions 
inline mousse::scalar mousse::LangmuirHinshelwoodReactionRate::operator()
(
  const scalar /*p*/,
  const scalar T,
  const scalarField& c
) const
{
  return
    A_[0]*exp(-Ta_[0]/T)/
    (
      T
      *sqr(1 + A_[1]*exp(-Ta_[1]/T)*c[co_] + A_[2]*exp(-Ta_[2]/T)*c[c3h6_])
      *(1 + A_[3]*exp(-Ta_[3]/T)*sqr(c[co_])*sqr(c[c3h6_]))
      *(1 + A_[4]*exp(-Ta_[4]/T)*pow(c[no_], 0.7))
    );
}


inline void mousse::LangmuirHinshelwoodReactionRate::write(Ostream& os) const
{
  FixedList<Tuple2<scalar, scalar>, n_> coeffs;
  FOR_ALL(coeffs, i) {
    coeffs[i].first() = A_[i];
    coeffs[i].second() = Ta_[i];
  }
  os.writeKeyword("coeffs") << coeffs << nl;
}


inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const LangmuirHinshelwoodReactionRate& lhrr
)
{
  os << token::BEGIN_LIST;
  for (int i=0; i<LangmuirHinshelwoodReactionRate::n_; i++) {
    os << token::SPACE
      << '(' << lhrr.A_[i] << token::SPACE << lhrr.Ta_[i] << ')';
  }
  os << token::END_LIST;
  return os;
}

#endif

