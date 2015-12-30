// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
  k0_(k0),
  kInf_(kInf),
  F_(F),
  thirdBodyEfficiencies_(tbes)
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
  k0_(species, is.readBegin("ChemicallyActivatedReactionRate(Istream&)")),
  kInf_(species, is),
  F_(is),
  thirdBodyEfficiencies_(species, is)
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
  k0_(species, dict),
  kInf_(species, dict),
  F_(dict),
  thirdBodyEfficiencies_(species, dict)
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
  os  << token::BEGIN_LIST
    << carr.k0_ << token::SPACE << carr.kInf_ << token::SPACE << carr.F_
    << token::END_LIST;
  return os;
}
