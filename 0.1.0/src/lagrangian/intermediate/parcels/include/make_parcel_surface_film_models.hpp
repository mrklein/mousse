// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_surface_film_models_hpp_
#define make_parcel_surface_film_models_hpp_

#include "_no_surface_film.hpp"

#define makeParcelSurfaceFilmModels(CloudType)                                \
                                                                              \
  makeSurfaceFilmModel(CloudType);                                            \
  makeSurfaceFilmModelType(NoSurfaceFilm, CloudType);

#endif
