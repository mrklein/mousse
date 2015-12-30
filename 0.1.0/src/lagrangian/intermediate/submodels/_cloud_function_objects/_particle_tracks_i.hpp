// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_tracks.hpp"
template<class CloudType>
inline mousse::label mousse::ParticleTracks<CloudType>::trackInterval() const
{
  return trackInterval_;
}
template<class CloudType>
inline mousse::label mousse::ParticleTracks<CloudType>::maxSamples() const
{
  return maxSamples_;
}
template<class CloudType>
inline const mousse::Switch& mousse::ParticleTracks<CloudType>::resetOnWrite() const
{
  return resetOnWrite_;
}
template<class CloudType>
inline const typename mousse::ParticleTracks<CloudType>::hitTableType&
mousse::ParticleTracks<CloudType>::faceHitCounter() const
{
  return faceHitCounter_;
}
template<class CloudType>
inline const mousse::Cloud<typename CloudType::parcelType>&
mousse::ParticleTracks<CloudType>::cloud() const
{
  return cloudPtr_();
}
