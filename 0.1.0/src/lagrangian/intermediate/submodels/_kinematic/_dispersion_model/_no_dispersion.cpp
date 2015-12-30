// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_dispersion.hpp"
// Constructors 
template<class CloudType>
mousse::NoDispersion<CloudType>::NoDispersion(const dictionary&, CloudType& owner)
:
  DispersionModel<CloudType>(owner)
{}
template<class CloudType>
mousse::NoDispersion<CloudType>::NoDispersion(const NoDispersion<CloudType>& dm)
:
  DispersionModel<CloudType>(dm.owner_)
{}
// Destructor 
template<class CloudType>
mousse::NoDispersion<CloudType>::~NoDispersion()
{}
// Member Functions 
template<class CloudType>
bool mousse::NoDispersion<CloudType>::active() const
{
  return false;
}
template<class CloudType>
mousse::vector mousse::NoDispersion<CloudType>::update
(
  const scalar,
  const label,
  const vector&,
  const vector& Uc,
  vector&,
  scalar&
)
{
  // Do nothing
  return Uc;
}
