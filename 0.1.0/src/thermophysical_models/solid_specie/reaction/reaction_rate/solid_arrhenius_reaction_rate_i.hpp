// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const scalar A,
  const scalar Ta,
  const scalar Tcrit
  //const scalar nReact
)
:
  A_(A),
  Ta_(Ta),
  Tcrit_(Tcrit)
{}
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const speciesTable&,
  Istream& is
)
:
  A_(readScalar(is.readBegin("solidArrheniusReaction(Istream&)"))),
  Ta_(readScalar(is)),
  Tcrit_(readScalar(is))
{}
inline mousse::solidArrheniusReactionRate::solidArrheniusReactionRate
(
  const speciesTable&,
  const dictionary& dict
)
:
  A_(readScalar(dict.lookup("A"))),
  Ta_(readScalar(dict.lookup("Ta"))),
  Tcrit_(readScalar(dict.lookup("Tcrit")))
{}
// Member Functions 
inline mousse::scalar mousse::solidArrheniusReactionRate::operator()
(
  const scalar,
  const scalar T,
  const scalarField&
) const
{
  scalar ak = A_;
  if (T < Tcrit_)
  {
    ak *= 0.0;
  }
  else
  {
    ak *= exp(-Ta_/T);
  }
  return ak;
}
inline void mousse::solidArrheniusReactionRate::write(Ostream& os) const
{
  os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
  os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
  os.writeKeyword("Tcrit") << Tcrit_ << token::END_STATEMENT << nl;
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const solidArrheniusReactionRate& arr
)
{
  os  << token::BEGIN_LIST
    << arr.A_ << token::SPACE << arr.Ta_ << token::SPACE << arr.Tcrit_
    << token::END_LIST;
  return os;
}
