// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class ParcelType>
template<class CloudType>
inline mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>(cloud),
  rhoInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.rho()
    )
  ),
  UInterp_
  (
    interpolation<vector>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.U()
    )
  ),
  muInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.mu()
    )
  ),
  g_(cloud.g().value()),
  part_(part)
{}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::rhoInterp() const
{
  return rhoInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::UInterp() const
{
  return UInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::muInterp() const
{
  return muInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::vector&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::g() const
{
  return g_;
}
template<class ParcelType>
template<class CloudType>
inline typename mousse::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::part() const
{
  return part_;
}
template<class ParcelType>
template<class CloudType>
inline typename mousse::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart&
mousse::KinematicParcel<ParcelType>::TrackingData<CloudType>::part()
{
  return part_;
}
