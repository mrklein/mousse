// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction()
{}
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction(Istream&)
{}
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction
(
  const dictionary& dict
)
{}
// Member Functions 
inline mousse::scalar mousse::LindemannFallOffFunction::operator()
(
  const scalar,
  const scalar
) const
{
  return 1.0;
}
inline void mousse::LindemannFallOffFunction::write(Ostream& os) const
{}
// Ostream Operator 
inline mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::LindemannFallOffFunction&
)
{
  return os;
}
