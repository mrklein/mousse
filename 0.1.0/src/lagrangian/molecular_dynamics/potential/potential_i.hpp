// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline mousse::label mousse::potential::nIds() const
{
  return idList_.size();
}
inline const mousse::List<mousse::word>& mousse::potential::idList() const
{
  return idList_;
}
inline const mousse::List<mousse::word>& mousse::potential::siteIdList() const
{
  return siteIdList_;
}
inline mousse::scalar mousse::potential::potentialEnergyLimit() const
{
  return potentialEnergyLimit_;
}
inline mousse::label mousse::potential::nPairPotentials() const
{
  return pairPotentials_.size();
}
inline const mousse::labelList& mousse::potential::removalOrder() const
{
  return removalOrder_;
}
inline const mousse::pairPotentialList& mousse::potential::pairPotentials() const
{
  return pairPotentials_;
}
inline const mousse::tetherPotentialList&
mousse::potential::tetherPotentials() const
{
  return tetherPotentials_;
}
inline const mousse::vector& mousse::potential::gravity() const
{
  return gravity_;
}
