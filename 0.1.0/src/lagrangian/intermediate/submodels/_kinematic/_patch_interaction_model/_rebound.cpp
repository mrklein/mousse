// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_rebound.hpp"
// Constructors 
template<class CloudType>
mousse::Rebound<CloudType>::Rebound
(
  const dictionary& dict,
  CloudType& cloud
)
:
  PatchInteractionModel<CloudType>(dict, cloud, typeName),
  UFactor_(readScalar(this->coeffDict().lookup("UFactor")))
{}
template<class CloudType>
mousse::Rebound<CloudType>::Rebound(const Rebound<CloudType>& pim)
:
  PatchInteractionModel<CloudType>(pim),
  UFactor_(pim.UFactor_)
{}
// Destructor 
template<class CloudType>
mousse::Rebound<CloudType>::~Rebound()
{}
// Member Functions 
template<class CloudType>
bool mousse::Rebound<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const polyPatch& pp,
  bool& keepParticle,
  const scalar trackFraction,
  const tetIndices& tetIs
)
{
  vector& U = p.U();
  keepParticle = true;
  p.active() = true;
  vector nw;
  vector Up;
  this->owner().patchData(p, pp, trackFraction, tetIs, nw, Up);
  // Calculate motion relative to patch velocity
  U -= Up;
  scalar Un = U & nw;
  if (Un > 0.0)
  {
    U -= UFactor_*2.0*Un*nw;
  }
  // Return velocity to global space
  U += Up;
  return true;
}
