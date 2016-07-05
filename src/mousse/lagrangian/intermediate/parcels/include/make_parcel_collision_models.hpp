#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_COLLISION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_PARCEL_COLLISION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_collision.hpp"
#include "_pair_collision.hpp"
#include "_pair_spring_slider_dashpot.hpp"
#include "_wall_spring_slider_dashpot.hpp"
#include "_wall_local_spring_slider_dashpot.hpp"


#define MAKE_PARCEL_COLLISION_MODELS(CloudType)                               \
                                                                              \
  MAKE_COLLISION_MODEL(CloudType);                                            \
  MAKE_COLLISION_MODEL_TYPE(NoCollision, CloudType);                          \
  MAKE_COLLISION_MODEL_TYPE(PairCollision, CloudType);                        \
                                                                              \
  MAKE_PAIR_MODEL(CloudType);                                                 \
  MAKE_PAIR_MODEL_TYPE(PairSpringSliderDashpot, CloudType);                   \
                                                                              \
  MAKE_WALL_MODEL(CloudType);                                                 \
  MAKE_WALL_MODEL_TYPE(WallSpringSliderDashpot, CloudType);                   \
  MAKE_WALL_MODEL_TYPE(WallLocalSpringSliderDashpot, CloudType);

#endif

