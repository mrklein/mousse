// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_surface_film_models_hpp_
#define make_parcel_surface_film_models_hpp_

#include "_no_surface_film.hpp"

#define MAKE_PARCEL_SURFACE_FILM_MODELS(CloudType)                            \
                                                                              \
  MAKE_SURFACE_FILM_MODEL(CloudType);                                         \
  MAKE_SURFACE_FILM_MODEL_TYPE(NoSurfaceFilm, CloudType);

#endif
