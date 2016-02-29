#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_MPPIC_PARCEL_DAMPING_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_MPPIC_PARCEL_DAMPING_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_damping_model.hpp"
#include "_no_damping.hpp"
#include "_relaxation.hpp"
#define MAKE_MPPIC_PARCEL_DAMPING_MODELS(CloudType)                           \
                                                                              \
  MAKE_DAMPING_MODEL(CloudType);                                              \
                                                                              \
  MAKE_DAMPING_MODEL_TYPE(NoDamping, CloudType);                              \
  MAKE_DAMPING_MODEL_TYPE(Relaxation, CloudType);
#endif
