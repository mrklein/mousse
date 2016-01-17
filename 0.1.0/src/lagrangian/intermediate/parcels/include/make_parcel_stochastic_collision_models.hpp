// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_stochastic_collision_models_hpp_
#define make_parcel_stochastic_collision_models_hpp_

#include "_no_stochastic_collision.hpp"

#define MAKE_PARCEL_STOCHASTIC_COLLISION_MODELS(CloudType)                    \
                                                                              \
  MAKE_STOCHASTIC_COLLISION_MODEL(CloudType);                                 \
  MAKE_STOCHASTIC_COLLISION_MODEL_TYPE(NoStochasticCollision, CloudType);

#endif
