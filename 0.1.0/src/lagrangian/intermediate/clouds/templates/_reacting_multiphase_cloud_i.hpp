// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class CloudType>
inline const mousse::ReactingMultiphaseCloud<CloudType>&
mousse::ReactingMultiphaseCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ReactingMultiphaseCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ReactingMultiphaseCloud<CloudType>::constProps()
{
  return constProps_;
}
template<class CloudType>
inline const mousse::DevolatilisationModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::devolatilisation() const
{
  return devolatilisationModel_;
}
template<class CloudType>
inline mousse::DevolatilisationModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::devolatilisation()
{
  return devolatilisationModel_();
}
template<class CloudType>
inline const mousse::SurfaceReactionModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::surfaceReaction() const
{
  return surfaceReactionModel_;
}
template<class CloudType>
inline mousse::SurfaceReactionModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::surfaceReaction()
{
  return surfaceReactionModel_();
}
