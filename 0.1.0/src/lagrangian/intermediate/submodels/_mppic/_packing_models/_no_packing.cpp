// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_packing.hpp"
// Constructors 
template<class CloudType>
mousse::PackingModels::NoPacking<CloudType>::NoPacking
(
  const dictionary& dict,
  CloudType& owner
)
:
  PackingModel<CloudType>(owner)
{}
template<class CloudType>
mousse::PackingModels::NoPacking<CloudType>::NoPacking
(
  const NoPacking<CloudType>& cm
)
:
  PackingModel<CloudType>(cm)
{}
// Destructor 
template<class CloudType>
mousse::PackingModels::NoPacking<CloudType>::~NoPacking()
{}
// Member Functions 
template<class CloudType>
mousse::vector mousse::PackingModels::NoPacking<CloudType>::velocityCorrection
(
  typename CloudType::parcelType& p,
  const scalar deltaT
) const
{
  return vector::zero;
}
template<class CloudType>
bool mousse::PackingModels::NoPacking<CloudType>::active() const
{
  return false;
}
