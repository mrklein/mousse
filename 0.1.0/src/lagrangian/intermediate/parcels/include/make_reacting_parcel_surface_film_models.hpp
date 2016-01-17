// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_parcel_surface_film_models_hpp_
#define make_reacting_parcel_surface_film_models_hpp_

#include "_no_surface_film.hpp"
#include "_thermo_surface_film.hpp"

#define MAKE_REACTING_PARCEL_SURFACE_FILM_MODELS(CloudType)                   \
                                                                              \
  MAKE_SURFACE_FILM_MODEL(CloudType);                                         \
  MAKE_SURFACE_FILM_MODEL_TYPE(NoSurfaceFilm, CloudType);                     \
  MAKE_SURFACE_FILM_MODEL_TYPE(ThermoSurfaceFilm, CloudType);

#endif
