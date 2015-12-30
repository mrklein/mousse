// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_surface_reaction_models_hpp_
#define make_reacting_multiphase_parcel_surface_reaction_models_hpp_

#include "_no_surface_reaction.hpp"

#define makeReactingMultiphaseParcelSurfaceReactionModels(CloudType)          \
                                                                              \
  makeSurfaceReactionModel(CloudType);                                        \
  makeSurfaceReactionModelType(NoSurfaceReaction, CloudType);

#endif
