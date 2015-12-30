// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_isotropy.hpp"
// Constructors 
template<class CloudType>
mousse::IsotropyModels::NoIsotropy<CloudType>::NoIsotropy
(
  const dictionary& dict,
  CloudType& owner
)
:
  IsotropyModel<CloudType>(owner)
{}
template<class CloudType>
mousse::IsotropyModels::NoIsotropy<CloudType>::NoIsotropy
(
  const NoIsotropy<CloudType>& cm
)
:
  IsotropyModel<CloudType>(cm)
{}
// Destructor 
template<class CloudType>
mousse::IsotropyModels::NoIsotropy<CloudType>::~NoIsotropy()
{}
// Member Functions 
template<class CloudType>
void mousse::IsotropyModels::NoIsotropy<CloudType>::calculate()
{
  // do nothing
}
template<class CloudType>
bool mousse::IsotropyModels::NoIsotropy<CloudType>::active() const
{
  return false;
}
