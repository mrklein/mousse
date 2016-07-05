// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_stochastic_collision.hpp"


// Protected Member Functions 
template<class CloudType>
void mousse::NoStochasticCollision<CloudType>::collide(const scalar)
{
  // do nothing
}


// Constructors 
template<class CloudType>
mousse::NoStochasticCollision<CloudType>::NoStochasticCollision
(
  const dictionary& /*dict*/,
  CloudType& owner
)
:
  StochasticCollisionModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoStochasticCollision<CloudType>::NoStochasticCollision
(
  const NoStochasticCollision<CloudType>& cm
)
:
  StochasticCollisionModel<CloudType>{cm}
{}


// Destructor 
template<class CloudType>
mousse::NoStochasticCollision<CloudType>::~NoStochasticCollision()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoStochasticCollision<CloudType>::active() const
{
  return false;
}

