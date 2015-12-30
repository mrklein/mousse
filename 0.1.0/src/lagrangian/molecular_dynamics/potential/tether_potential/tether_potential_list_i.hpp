// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline mousse::label mousse::tetherPotentialList::tetherPotentialIndex
(
  const label a
) const
{
  label index = idMap_[a];
  if (index == -1 || a >= idMap_.size())
  {
    FatalErrorIn
    (
      "mousse::tetherPotentialList::tetherPotentialIndex(const label a)"
    )
    << "Attempting to access an undefined tetherPotential."
    << abort(FatalError);
    return -1;
  }
  else
  {
    return index;
  }
}
