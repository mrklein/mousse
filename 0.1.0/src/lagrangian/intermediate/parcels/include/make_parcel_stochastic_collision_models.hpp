// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_stochastic_collision_models_hpp_
#define make_parcel_stochastic_collision_models_hpp_

#include "_no_stochastic_collision.hpp"

#define makeParcelStochasticCollisionModels(CloudType)                        \
                                                                              \
  makeStochasticCollisionModel(CloudType);                                    \
  makeStochasticCollisionModelType(NoStochasticCollision, CloudType);

#endif
