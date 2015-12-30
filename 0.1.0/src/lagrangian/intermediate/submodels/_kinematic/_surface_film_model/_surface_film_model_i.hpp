// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_surface_film_model.hpp"
// Member Functions 
template<class CloudType>
const mousse::dimensionedVector& mousse::SurfaceFilmModel<CloudType>::g() const
{
  return g_;
}
template<class CloudType>
mousse::label& mousse::SurfaceFilmModel<CloudType>::nParcelsTransferred()
{
  return nParcelsTransferred_;
}
template<class CloudType>
mousse::label mousse::SurfaceFilmModel<CloudType>::nParcelsTransferred() const
{
  return nParcelsTransferred_;
}
template<class CloudType>
mousse::label& mousse::SurfaceFilmModel<CloudType>::nParcelsInjected()
{
  return nParcelsInjected_;
}
template<class CloudType>
mousse::label mousse::SurfaceFilmModel<CloudType>::nParcelsInjected() const
{
  return nParcelsInjected_;
}
