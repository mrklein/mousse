// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_damping.hpp"


// Constructors 
template<class CloudType>
mousse::DampingModels::NoDamping<CloudType>::NoDamping
(
  const dictionary&,
  CloudType& owner
)
:
  DampingModel<CloudType>{owner}
{}


template<class CloudType>
mousse::DampingModels::NoDamping<CloudType>::NoDamping
(
  const NoDamping<CloudType>& cm
)
:
  DampingModel<CloudType>{cm}
{}


// Destructor 
template<class CloudType>
mousse::DampingModels::NoDamping<CloudType>::~NoDamping()
{}


// Member Functions 
template<class CloudType>
mousse::vector mousse::DampingModels::NoDamping<CloudType>::velocityCorrection
(
  typename CloudType::parcelType& /*p*/,
  const scalar /*deltaT*/
) const
{
  return vector::zero;
}


template<class CloudType>
bool mousse::DampingModels::NoDamping<CloudType>::active() const
{
  return false;
}

