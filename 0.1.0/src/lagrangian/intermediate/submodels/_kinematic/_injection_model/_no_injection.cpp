// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_injection.hpp"
#include "add_to_run_time_selection_table.hpp"
// Constructors 
template<class CloudType>
mousse::NoInjection<CloudType>::NoInjection
(
  const dictionary&,
  CloudType& owner,
  const word&
)
:
  InjectionModel<CloudType>(owner)
{}
template<class CloudType>
mousse::NoInjection<CloudType>::NoInjection(const NoInjection<CloudType>& im)
:
  InjectionModel<CloudType>(im.owner_)
{}
// Destructor 
template<class CloudType>
mousse::NoInjection<CloudType>::~NoInjection()
{}
// Member Functions 
template<class CloudType>
bool mousse::NoInjection<CloudType>::active() const
{
  return false;
}
template<class CloudType>
mousse::scalar mousse::NoInjection<CloudType>::timeEnd() const
{
  return 0.0;
}
template<class CloudType>
mousse::label mousse::NoInjection<CloudType>::parcelsToInject
(
  const scalar,
  const scalar
)
{
  return 0;
}
template<class CloudType>
mousse::scalar mousse::NoInjection<CloudType>::volumeToInject
(
  const scalar,
  const scalar
)
{
  return 0.0;
}
template<class CloudType>
void mousse::NoInjection<CloudType>::setPositionAndCell
(
  const label,
  const label,
  const scalar,
  vector&,
  label&,
  label&,
  label&
)
{}
template<class CloudType>
void mousse::NoInjection<CloudType>::setProperties
(
  const label,
  const label,
  const scalar,
  typename CloudType::parcelType& parcel
)
{
  // set particle velocity
  parcel.U() = vector::zero;
  // set particle diameter
  parcel.d() = 0.0;
}
template<class CloudType>
bool mousse::NoInjection<CloudType>::fullyDescribed() const
{
  return false;
}
template<class CloudType>
bool mousse::NoInjection<CloudType>::validInjection(const label)
{
  return false;
}
