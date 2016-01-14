// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_surface_reaction_models_hpp_
#define make_reacting_multiphase_parcel_surface_reaction_models_hpp_

#include "_no_surface_reaction.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_SURFACE_REACTION_MODELS(CloudType)    \
                                                                              \
  MAKE_SURFACE_REACTION_MODEL(CloudType);                                     \
  MAKE_SURFACE_REACTION_MODEL_TYPE(NoSurfaceReaction, CloudType);

#endif
