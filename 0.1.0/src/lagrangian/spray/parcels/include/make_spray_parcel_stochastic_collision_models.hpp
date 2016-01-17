// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_spray_parcel_stochastic_collision_models_hpp_
#define make_spray_parcel_stochastic_collision_models_hpp_

#include "_no_stochastic_collision.hpp"
#include "_o_rourke_collision.hpp"
#include "_trajectory_collision.hpp"

#define MAKE_SPRAY_PARCEL_STOCHASTIC_COLLISION_MODELS(CloudType)              \
                                                                              \
  MAKE_STOCHASTIC_COLLISION_MODEL(CloudType);                                 \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(NoStochasticCollision, CloudType);     \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(ORourkeCollision, CloudType);          \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(TrajectoryCollision, CloudType);
#endif
