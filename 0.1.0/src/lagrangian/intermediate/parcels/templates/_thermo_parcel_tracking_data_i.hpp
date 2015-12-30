// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class ParcelType>
template<class CloudType>
inline mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>(cloud, part),
  Cp_(cloud.thermo().thermo().Cp()),
  kappa_(cloud.thermo().thermo().kappa()),
  TInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      cloud.T()
    )
  ),
  CpInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      Cp_
    )
  ),
  kappaInterp_
  (
    interpolation<scalar>::New
    (
      cloud.solution().interpolationSchemes(),
      kappa_
    )
  ),
  GInterp_(NULL)
{
  if (cloud.radiation())
  {
    GInterp_.reset
    (
      interpolation<scalar>::New
      (
        cloud.solution().interpolationSchemes(),
        cloud.mesh().objectRegistry::template
          lookupObject<volScalarField>("G")
      ).ptr()
    );
  }
}
template<class ParcelType>
template<class CloudType>
inline const mousse::volScalarField&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::Cp() const
{
  return Cp_;
}
template<class ParcelType>
template<class CloudType>
inline const mousse::volScalarField&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappa() const
{
  return kappa_;
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::TInterp() const
{
  return TInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::CpInterp() const
{
  return CpInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::kappaInterp() const
{
  return kappaInterp_();
}
template<class ParcelType>
template<class CloudType>
inline const mousse::interpolation<mousse::scalar>&
mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::GInterp() const
{
  if (!GInterp_.valid())
  {
    FatalErrorIn
    (
      "inline const mousse::interpolation<mousse::scalar>&"
      "mousse::ThermoParcel<ParcelType>::TrackingData<CloudType>::"
      "GInterp() const"
    )   << "Radiation G interpolation object not set"
      << abort(FatalError);
  }
  return GInterp_();
}
