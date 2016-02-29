#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_SURFACE_REACTION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_SURFACE_REACTION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_surface_reaction.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_SURFACE_REACTION_MODELS(CloudType)    \
                                                                              \
  MAKE_SURFACE_REACTION_MODEL(CloudType);                                     \
  MAKE_SURFACE_REACTION_MODEL_TYPE(NoSurfaceReaction, CloudType);

#endif
