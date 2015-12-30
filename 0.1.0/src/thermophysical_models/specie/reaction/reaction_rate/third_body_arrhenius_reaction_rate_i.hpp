// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::thirdBodyArrheniusReactionRate::thirdBodyArrheniusReactionRate
(
  const scalar A,
  const scalar beta,
  const scalar Ta,
  const thirdBodyEfficiencies& tbes
)
:
  ArrheniusReactionRate(A, beta, Ta),
  thirdBodyEfficiencies_(tbes)
{}
inline mousse::thirdBodyArrheniusReactionRate::thirdBodyArrheniusReactionRate
(
  const speciesTable& species,
  Istream& is
)
:
  ArrheniusReactionRate
  (
    species,
    is.readBegin("thirdBodyArrheniusReactionRate(Istream&)")
  ),
  thirdBodyEfficiencies_(species, is)
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
  (
    species,
    dict
  ),
  thirdBodyEfficiencies_(species, dict)
{}
// Member Functions 
inline mousse::scalar mousse::thirdBodyArrheniusReactionRate::operator()
(
  const scalar p,
  const scalar T,
  const scalarField& c
) const
{
  return
    thirdBodyEfficiencies_.M(c)
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
  os  << token::BEGIN_LIST
    << static_cast<const ArrheniusReactionRate&>(arr)
    << token::SPACE << arr.thirdBodyEfficiencies_
    << token::END_LIST;
  return os;
}
