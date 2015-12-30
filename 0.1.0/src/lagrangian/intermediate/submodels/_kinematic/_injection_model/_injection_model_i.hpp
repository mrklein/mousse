// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_injection_model.hpp"
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::timeStart() const
{
  return SOI_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::volumeTotal() const
{
  return volumeTotal_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::massTotal() const
{
  return massTotal_;
}
template<class CloudType>
mousse::scalar mousse::InjectionModel<CloudType>::massInjected() const
{
  return massInjected_;
}
template<class CloudType>
mousse::label mousse::InjectionModel<CloudType>::nInjections() const
{
  return nInjections_;
}
template<class CloudType>
mousse::label mousse::InjectionModel<CloudType>::parcelsAddedTotal() const
{
  return parcelsAddedTotal_;
}
