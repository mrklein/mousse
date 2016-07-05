// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_surface_film.hpp"
#include "add_to_run_time_selection_table.hpp"


// Constructors 
template<class CloudType>
mousse::NoSurfaceFilm<CloudType>::NoSurfaceFilm
(
  const dictionary&,
  CloudType& owner
)
:
  SurfaceFilmModel<CloudType>{owner}
{}


template<class CloudType>
mousse::NoSurfaceFilm<CloudType>::NoSurfaceFilm
(
  const NoSurfaceFilm<CloudType>& sfm
)
:
  SurfaceFilmModel<CloudType>{sfm.owner_}
{}


// Destructor 
template<class CloudType>
mousse::NoSurfaceFilm<CloudType>::~NoSurfaceFilm()
{}


// Member Functions 
template<class CloudType>
bool mousse::NoSurfaceFilm<CloudType>::active() const
{
  return false;
}


template<class CloudType>
bool mousse::NoSurfaceFilm<CloudType>::transferParcel
(
  parcelType&,
  const polyPatch&,
  bool&
)
{
  return false;
}


template<class CloudType>
void mousse::NoSurfaceFilm<CloudType>::setParcelProperties
(
  parcelType&,
  const label
) const
{}


template<class CloudType>
void mousse::NoSurfaceFilm<CloudType>::info(Ostream&)
{}

