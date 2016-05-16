// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_binary_collision_model.hpp"


// Constructors
template<class CloudType>
mousse::BinaryCollisionModel<CloudType>::BinaryCollisionModel(CloudType& owner)
:
  dict_{dictionary::null},
  owner_{owner},
  coeffDict_{dictionary::null}
{}


template<class CloudType>
mousse::BinaryCollisionModel<CloudType>::BinaryCollisionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  dict_{dict},
  owner_{owner},
  coeffDict_{dict.subDict(type + "Coeffs")}
{}


// Destructor
template<class CloudType>
mousse::BinaryCollisionModel<CloudType>::~BinaryCollisionModel()
{}


// Member Functions
template<class CloudType>
const CloudType&
mousse::BinaryCollisionModel<CloudType>::owner() const
{
  return owner_;
}


template<class CloudType>
CloudType&
mousse::BinaryCollisionModel<CloudType>::owner()
{
  return owner_;
}


template<class CloudType>
const mousse::dictionary&
mousse::BinaryCollisionModel<CloudType>::dict() const
{
  return dict_;
}


template<class CloudType>
const mousse::dictionary&
mousse::BinaryCollisionModel<CloudType>::coeffDict() const
{
  return coeffDict_;
}

#include "_binary_collision_model_new.ipp"

