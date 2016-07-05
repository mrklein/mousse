// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_binary_collision.hpp"
#include "constants.hpp"


using namespace mousse::constant::mathematical;


// Constructors 
template<class CloudType>
mousse::NoBinaryCollision<CloudType>::NoBinaryCollision
(
  const dictionary&,
  CloudType& cloud
)
:
  BinaryCollisionModel<CloudType>{cloud}
{}


// Destructor 
template<class CloudType>
mousse::NoBinaryCollision<CloudType>::~NoBinaryCollision()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoBinaryCollision<CloudType>::active() const
{
  return false;
}


template<class CloudType>
mousse::scalar mousse::NoBinaryCollision<CloudType>::sigmaTcR
(
  const typename CloudType::parcelType& /*pP*/,
  const typename CloudType::parcelType& /*pQ*/
) const
{
  FATAL_ERROR_IN
  (
    "mousse::scalar mousse::NoBinaryCollision<CloudType>::sigmaTcR"
    "("
      "const typename CloudType::parcelType&, "
      "const typename CloudType::parcelType"
    ") const"
  )
  << "sigmaTcR called on NoBinaryCollision model, this should "
  << "not happen, this is not an actual model." << nl
  << "Enclose calls to sigmaTcR within a if (binaryCollision().active()) "
  << " check."
  << abort(FatalError);
  return 0.0;
}


template<class CloudType>
void mousse::NoBinaryCollision<CloudType>::collide
(
  typename CloudType::parcelType& /*pP*/,
  typename CloudType::parcelType& /*pQ*/
)
{}

