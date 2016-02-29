#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_DISPERSION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_DISPERSION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_dispersion.hpp"
#define MAKE_PARCEL_DISPERSION_MODELS(CloudType)                              \
                                                                              \
  MAKE_DISPERSION_MODEL(CloudType);                                           \
  MAKE_DISPERSION_MODEL_TYPE(NoDispersion, CloudType);

#endif
