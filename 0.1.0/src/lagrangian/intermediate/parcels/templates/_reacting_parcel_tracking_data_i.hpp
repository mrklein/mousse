// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class ParcelType>
template<class CloudType>
inline mousse::ReactingParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>(cloud, part),
  pInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.p()
    )
  )
{}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ReactingParcel<ParcelType>::TrackingData<CloudType>::pInterp() const
{
  return pInterp_();
}
