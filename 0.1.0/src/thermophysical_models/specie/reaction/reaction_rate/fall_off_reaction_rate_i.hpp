// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ReactionRate, class FallOffFunction>
inline mousse::FallOffReactionRate<ReactionRate, FallOffFunction>::
FallOffReactionRate
(
  const ReactionRate& k0,
  const ReactionRate& kInf,
  const FallOffFunction& F,
  const thirdBodyEfficiencies& tbes
)
:
  k0_(k0),
  kInf_(kInf),
  F_(F),
  thirdBodyEfficiencies_(tbes)
{}
template<class ReactionRate, class FallOffFunction>
inline mousse::FallOffReactionRate<ReactionRate, FallOffFunction>::
FallOffReactionRate
(
  const speciesTable& species,
  Istream& is
)
:
  k0_(species, is.readBegin("FallOffReactionRate(Istream&)")),
  kInf_(species, is),
  F_(is),
  thirdBodyEfficiencies_(species, is)
{
  is.readEnd("FallOffReactionRate(Istream&)");
}
template<class ReactionRate, class FallOffFunction>
inline mousse::FallOffReactionRate<ReactionRate, FallOffFunction>::
FallOffReactionRate
(
  const speciesTable& species,
  const dictionary& dict
)
:
  k0_(species, dict.subDict("k0")),
  kInf_(species, dict.subDict("kInf")),
  F_(dict.subDict("F")),
  thirdBodyEfficiencies_(species, dict.subDict("thirdBodyEfficiencies"))
{}
// Member Functions 
template<class ReactionRate, class FallOffFunction>
inline mousse::scalar
mousse::FallOffReactionRate<ReactionRate, FallOffFunction>::operator()
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  scalar k0 = k0_(p, T, c);
  scalar kInf = kInf_(p, T, c);
  scalar Pr = k0*thirdBodyEfficiencies_.M(c)/kInf;
  return kInf*(Pr/(1 + Pr))*F_(T, Pr);
}
template<class ReactionRate, class FallOffFunction>
inline void mousse::FallOffReactionRate<ReactionRate, FallOffFunction>::write
(
  Ostream& os
) const
{
  os  << indent << "k0" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  k0_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
  os  << indent << "kInf" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  kInf_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
  os  << indent << "F" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  F_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
  os  << indent << "thirdBodyEfficiencies" << nl;
  os  << indent << token::BEGIN_BLOCK << nl;
  os  << incrIndent;
  thirdBodyEfficiencies_.write(os);
  os  << decrIndent;
  os  << indent << token::END_BLOCK << nl;
}
template<class ReactionRate, class FallOffFunction>
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const FallOffReactionRate<ReactionRate, FallOffFunction>& forr
)
{
  os  << token::BEGIN_LIST
    << forr.k0_ << token::SPACE
    << forr.kInf_ << token::SPACE
    << forr.F_ << token::SPACE
    << forr.thirdBodyEfficiencies_
    << token::END_LIST;
  return os;
}
