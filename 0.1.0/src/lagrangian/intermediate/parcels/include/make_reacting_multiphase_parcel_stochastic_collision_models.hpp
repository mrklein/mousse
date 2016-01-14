// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_stochastic_collision_models_hpp_
#define make_reacting_multiphase_parcel_stochastic_collision_models_hpp_

#include "_no_stochastic_collision.hpp"
#include "_suppression_collision.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_STOCHASTIC_COLLISION_MODELS(CloudType)\
                                                                              \
  MAKE_STOCHASTIC_COLLISION_MODEL(CloudType);                                 \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(NoStochasticCollision, CloudType);     \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(SuppressionCollision, CloudType);
#endif
