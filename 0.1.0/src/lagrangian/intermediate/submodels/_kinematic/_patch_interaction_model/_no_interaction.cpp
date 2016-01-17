// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_interaction.hpp"
// Constructors 
template<class CloudType>
mousse::NoInteraction<CloudType>::NoInteraction
(
  const dictionary&,
  CloudType& owner
)
:
  PatchInteractionModel<CloudType>(owner)
{}
template<class CloudType>
mousse::NoInteraction<CloudType>::NoInteraction
(
  const NoInteraction<CloudType>& pim
)
:
  PatchInteractionModel<CloudType>(pim)
{}
// Destructor 
template<class CloudType>
mousse::NoInteraction<CloudType>::~NoInteraction()
{}
// Member Functions 
template<class CloudType>
bool mousse::NoInteraction<CloudType>::active() const
{
  return false;
}
template<class CloudType>
bool mousse::NoInteraction<CloudType>::correct
(
  typename CloudType::parcelType& /*p*/,
  const polyPatch&,
  bool&,
  const scalar,
  const tetIndices&
)
{
  return false;
}
