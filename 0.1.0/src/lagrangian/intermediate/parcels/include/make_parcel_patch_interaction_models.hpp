// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_patch_interaction_models_hpp_
#define make_parcel_patch_interaction_models_hpp_

#include "_local_interaction.hpp"
#include "_no_interaction.hpp"
#include "_rebound.hpp"
#include "_standard_wall_interaction.hpp"
#include "_multi_interaction.hpp"

#define makeParcelPatchInteractionModels(CloudType)                           \
                                                                              \
  makePatchInteractionModel(CloudType);                                       \
                                                                              \
  makePatchInteractionModelType(LocalInteraction, CloudType);                 \
  makePatchInteractionModelType(NoInteraction, CloudType);                    \
  makePatchInteractionModelType(Rebound, CloudType);                          \
  makePatchInteractionModelType(StandardWallInteraction, CloudType);          \
  makePatchInteractionModelType(MultiInteraction, CloudType);

#endif
