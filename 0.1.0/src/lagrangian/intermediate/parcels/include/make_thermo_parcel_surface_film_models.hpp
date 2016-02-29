#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_THERMO_PARCEL_SURFACE_FILM_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_THERMO_PARCEL_SURFACE_FILM_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_surface_film.hpp"
#include "_thermo_surface_film.hpp"

#define MAKE_PARCEL_SURFACE_FILM_MODELS(CloudType)                            \
                                                                              \
  MAKE_SURFACE_FILM_MODEL(CloudType);                                         \
                                                                              \
  MAKE_SURFACE_FILM_MODEL_TYPE(NoSurfaceFilm, CloudType);                     \
  MAKE_SURFACE_FILM_MODEL_TYPE(ThermoSurfaceFilm, CloudType);

#endif
