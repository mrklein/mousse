// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_collision_model.hpp"
// Constructors 
template<class CloudType>
mousse::CollisionModel<CloudType>::CollisionModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>(owner)
{}
template<class CloudType>
mousse::CollisionModel<CloudType>::CollisionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type)
{}
template<class CloudType>
mousse::CollisionModel<CloudType>::CollisionModel
(
  const CollisionModel<CloudType>& cm
)
:
  CloudSubModelBase<CloudType>(cm)
{}
// Destructor 
template<class CloudType>
mousse::CollisionModel<CloudType>::~CollisionModel()
{}
#include "_collision_model_new.cpp"
