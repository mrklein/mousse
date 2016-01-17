// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_dispersion_models_hpp_
#define make_parcel_dispersion_models_hpp_

#include "_no_dispersion.hpp"
#define MAKE_PARCEL_DISPERSION_MODELS(CloudType)                              \
                                                                              \
  MAKE_DISPERSION_MODEL(CloudType);                                           \
  MAKE_DISPERSION_MODEL_TYPE(NoDispersion, CloudType);

#endif
