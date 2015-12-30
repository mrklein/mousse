// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class CloudType>
inline const mousse::CollidingCloud<CloudType>&
mousse::CollidingCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::CollidingCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline const mousse::CollisionModel<mousse::CollidingCloud<CloudType> >&
mousse::CollidingCloud<CloudType>::collision() const
{
  return collisionModel_();
}
template<class CloudType>
inline mousse::CollisionModel<mousse::CollidingCloud<CloudType> >&
mousse::CollidingCloud<CloudType>::collision()
{
  return collisionModel_();
}
template<class CloudType>
inline mousse::scalar
mousse::CollidingCloud<CloudType>::rotationalKineticEnergyOfSystem() const
{
  scalar rotationalKineticEnergy = 0.0;
  forAllConstIter(typename CollidingCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    rotationalKineticEnergy +=
      p.nParticle()*0.5*p.momentOfInertia()*(p.omega() & p.omega());
  }
  return rotationalKineticEnergy;
}
