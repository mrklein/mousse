#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_PATCH_INTERACTION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_PATCH_INTERACTION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_local_interaction.hpp"
#include "_no_interaction.hpp"
#include "_rebound.hpp"
#include "_standard_wall_interaction.hpp"
#include "_multi_interaction.hpp"


#define MAKE_PARCEL_PATCH_INTERACTION_MODELS(CloudType)                       \
                                                                              \
  MAKE_PATCH_INTERACTION_MODEL(CloudType);                                    \
                                                                              \
  MAKE_PATCH_INTERACTION_MODEL_TYPE(LocalInteraction, CloudType);             \
  MAKE_PATCH_INTERACTION_MODEL_TYPE(NoInteraction, CloudType);                \
  MAKE_PATCH_INTERACTION_MODEL_TYPE(Rebound, CloudType);                      \
  MAKE_PATCH_INTERACTION_MODEL_TYPE(StandardWallInteraction, CloudType);      \
  MAKE_PATCH_INTERACTION_MODEL_TYPE(MultiInteraction, CloudType);

#endif

