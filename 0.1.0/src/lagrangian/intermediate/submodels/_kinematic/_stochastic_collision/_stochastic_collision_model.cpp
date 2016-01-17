// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_stochastic_collision_model.hpp"
// Constructors 
template<class CloudType>
mousse::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>{owner}
{}
template<class CloudType>
mousse::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
  const StochasticCollisionModel<CloudType>& cm
)
:
  CloudSubModelBase<CloudType>{cm}
{}
template<class CloudType>
mousse::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type}
{}
// Destructor 
template<class CloudType>
mousse::StochasticCollisionModel<CloudType>::~StochasticCollisionModel()
{}
// Member Functions 
template<class CloudType>
void mousse::StochasticCollisionModel<CloudType>::update(const scalar dt)
{
  if (this->active())
  {
    this->collide(dt);
  }
}
#include "_stochastic_collision_model_new.cpp"
