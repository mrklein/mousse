#ifndef LAGRANGIAN_SPRAY_PARCELS_INCLUDE_MAKE_SPRAY_PARCEL_STOCHASTIC_COLLISION_MODELS_HPP_
#define LAGRANGIAN_SPRAY_PARCELS_INCLUDE_MAKE_SPRAY_PARCEL_STOCHASTIC_COLLISION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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

