// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline mousse::label mousse::pairPotentialList::pairPotentialIndex
(
  const label a,
  const label b
) const
{
  label index;
  if (a < b)
  {
    index = a*(2*nIds_ - a - 1)/2 + b;
  }
  else
  {
    index = b*(2*nIds_ - b - 1)/2 + a;
  }
  if (index > size() - 1)
  {
    FatalErrorIn("mousse::pairPotentialList::pairPotentialIndex ")
      << "Attempting to access a pairPotential with too high an index."
      << nl << "a = " << a << ", b = " << b << ", index = " << index
      << nl << "max index = " << size() - 1
      << nl << abort(FatalError);
  }
  return index;
}
// Member Functions 
inline mousse::scalar mousse::pairPotentialList::rCutMax() const
{
  return rCutMax_;
}
inline mousse::scalar mousse::pairPotentialList::rCutMaxSqr() const
{
  return rCutMaxSqr_;
}
inline const mousse::pairPotential& mousse::pairPotentialList::electrostatic() const
{
  return electrostaticPotential_;
}
