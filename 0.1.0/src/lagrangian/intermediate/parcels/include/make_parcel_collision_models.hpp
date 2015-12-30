// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_collision_models_hpp_
#define make_parcel_collision_models_hpp_

#include "_no_collision.hpp"
#include "_pair_collision.hpp"
#include "_pair_spring_slider_dashpot.hpp"
#include "_wall_spring_slider_dashpot.hpp"
#include "_wall_local_spring_slider_dashpot.hpp"

#define makeParcelCollisionModels(CloudType)                                  \
                                                                              \
  makeCollisionModel(CloudType);                                              \
  makeCollisionModelType(NoCollision, CloudType);                             \
  makeCollisionModelType(PairCollision, CloudType);                           \
                                                                              \
  makePairModel(CloudType);                                                   \
  makePairModelType(PairSpringSliderDashpot, CloudType);                      \
                                                                              \
  makeWallModel(CloudType);                                                   \
  makeWallModelType(WallSpringSliderDashpot, CloudType);                      \
  makeWallModelType(WallLocalSpringSliderDashpot, CloudType);

#endif
