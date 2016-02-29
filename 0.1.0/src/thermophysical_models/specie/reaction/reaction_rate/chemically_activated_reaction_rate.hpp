#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_CHEMICALLY_ACTIVATED_REACTION_RATE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_CHEMICALLY_ACTIVATED_REACTION_RATE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ChemicallyActivatedReactionRate
// Description
//   General class for handling chemically-activated bimolecular reactions.
#include "third_body_efficiencies.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class ReactionRate, class ChemicallyActivationFunction>
class ChemicallyActivatedReactionRate;
template<class ReactionRate, class ChemicallyActivationFunction>
inline Ostream& operator<<
(
  Ostream&,
  const ChemicallyActivatedReactionRate
    <ReactionRate, ChemicallyActivationFunction>&
);
template<class ReactionRate, class ChemicallyActivationFunction>
class ChemicallyActivatedReactionRate
{
  // Private data
    ReactionRate k0_;
    ReactionRate kInf_;
    ChemicallyActivationFunction F_;
    thirdBodyEfficiencies thirdBodyEfficiencies_;
public:
  // Constructors
    //- Construct from components
    inline ChemicallyActivatedReactionRate
    (
      const ReactionRate& k0,
      const ReactionRate& kInf,
      const ChemicallyActivationFunction& F,
      const thirdBodyEfficiencies& tbes
    );
    //- Construct from Istream
    inline ChemicallyActivatedReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline ChemicallyActivatedReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return ReactionRate::type()
        + ChemicallyActivationFunction::type()
        + "ChemicallyActivated";
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
    friend Ostream& operator<< <ReactionRate, ChemicallyActivationFunction>
    (
      Ostream&,
      const ChemicallyActivatedReactionRate
        <ReactionRate, ChemicallyActivationFunction>&
    );
};
}  // namespace mousse

template<class ReactionRate, class ChemicallyActivationFunction>
inline mousse::ChemicallyActivatedReactionRate
<
  ReactionRate,
  ChemicallyActivationFunction
>::ChemicallyActivatedReactionRate
(
  const ReactionRate& k0,
  const ReactionRate& kInf,
  const ChemicallyActivationFunction& F,
  const thirdBodyEfficiencies& tbes
)
:
  k0_{k0},
  kInf_{kInf},
  F_{F},
  thirdBodyEfficiencies_{tbes}
{}
template<class ReactionRate, class ChemicallyActivationFunction>
inline mousse::ChemicallyActivatedReactionRate
<
  ReactionRate,
  ChemicallyActivationFunction
>::ChemicallyActivatedReactionRate
(
  const speciesTable& species,
  Istream& is
)
:
  k0_{species, is.readBegin("ChemicallyActivatedReactionRate(Istream&)")},
  kInf_{species, is},
  F_{is},
  thirdBodyEfficiencies_{species, is}
{
  is.readEnd("ChemicallyActivatedReactionRate(Istream&)");
}
template<class ReactionRate, class ChemicallyActivationFunction>
inline mousse::ChemicallyActivatedReactionRate
<
  ReactionRate,
  ChemicallyActivationFunction
>::ChemicallyActivatedReactionRate
(
  const speciesTable& species,
  const dictionary& dict
)
:
  k0_{species, dict},
  kInf_{species, dict},
  F_{dict},
  thirdBodyEfficiencies_{species, dict}
{}
// Member Functions 
template<class ReactionRate, class ChemicallyActivationFunction>
inline mousse::scalar mousse::ChemicallyActivatedReactionRate
<
  ReactionRate,
  ChemicallyActivationFunction
>::operator()
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  scalar k0 = k0_(p, T, c);
  scalar kInf = kInf_(p, T, c);
  scalar Pr = k0*thirdBodyEfficiencies_.M(c)/kInf;
  return k0*(1/(1 + Pr))*F_(T, Pr);
}
template<class ReactionRate, class ChemicallyActivationFunction>
inline void mousse::ChemicallyActivatedReactionRate
<
  ReactionRate,
  ChemicallyActivationFunction
>::write(Ostream& os) const
{
  k0_.write(os);
  kInf_.write(os);
  F_.write(os);
  thirdBodyEfficiencies_.write(os);
}
template<class ReactionRate, class ChemicallyActivationFunction>
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const ChemicallyActivatedReactionRate
    <ReactionRate, ChemicallyActivationFunction>& carr
)
{
  os<< token::BEGIN_LIST
    << carr.k0_ << token::SPACE << carr.kInf_ << token::SPACE << carr.F_
    << token::END_LIST;
  return os;
}
#endif
