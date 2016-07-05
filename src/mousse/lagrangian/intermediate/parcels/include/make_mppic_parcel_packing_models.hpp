#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_MPPIC_PARCEL_PACKING_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_MPPIC_PARCEL_PACKING_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_packing.hpp"
#include "_explicit.hpp"
#include "_implicit.hpp"


#define MAKE_MPPIC_PARCEL_PACKING_MODELS(CloudType)                           \
                                                                              \
  MAKE_PACKING_MODEL(CloudType);                                              \
                                                                              \
  MAKE_PACKING_MODEL_TYPE(NoPacking, CloudType);                              \
  MAKE_PACKING_MODEL_TYPE(Explicit, CloudType);                               \
  MAKE_PACKING_MODEL_TYPE(Implicit, CloudType);

#endif
