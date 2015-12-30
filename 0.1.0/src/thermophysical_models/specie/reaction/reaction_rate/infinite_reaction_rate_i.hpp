// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::infiniteReactionRate::infiniteReactionRate()
{}
inline mousse::infiniteReactionRate::infiniteReactionRate
(
  const speciesTable&,
  const dictionary&
)
{}
inline void mousse::infiniteReactionRate::write(Ostream& os) const
{}
// Member Functions 
inline mousse::scalar mousse::infiniteReactionRate::operator()
(
  const scalar p,
  const scalar,
  const scalarField&
) const
{
  return (1);
}
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const infiniteReactionRate& rr
)
{
  os  << token::BEGIN_LIST
    << token::END_LIST;
  return os;
}
