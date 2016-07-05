// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_collision.hpp"


// Constructors 
template<class CloudType>
mousse::NoCollision<CloudType>::NoCollision
(
  const dictionary&,
  CloudType& owner
)
:
  CollisionModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoCollision<CloudType>::NoCollision
(
  const NoCollision<CloudType>& cm
)
:
  CollisionModel<CloudType>{cm}
{}


// Destructor 
template<class CloudType>
mousse::NoCollision<CloudType>::~NoCollision()
{}


// Member Functions 
template<class CloudType>
mousse::label mousse::NoCollision<CloudType>::nSubCycles() const
{
  return 1;
}


template<class CloudType>
bool mousse::NoCollision<CloudType>::active() const
{
  return false;
}


template<class CloudType>
bool mousse::NoCollision<CloudType>::controlsWallInteraction() const
{
  return false;
}


template<class CloudType>
void mousse::NoCollision<CloudType>::collide()
{}

