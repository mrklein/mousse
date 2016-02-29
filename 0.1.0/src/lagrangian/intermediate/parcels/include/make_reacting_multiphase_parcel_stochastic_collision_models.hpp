#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_STOCHASTIC_COLLISION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_STOCHASTIC_COLLISION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_stochastic_collision.hpp"
#include "_suppression_collision.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_STOCHASTIC_COLLISION_MODELS(CloudType)\
                                                                              \
  MAKE_STOCHASTIC_COLLISION_MODEL(CloudType);                                 \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(NoStochasticCollision, CloudType);     \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(SuppressionCollision, CloudType);
#endif
