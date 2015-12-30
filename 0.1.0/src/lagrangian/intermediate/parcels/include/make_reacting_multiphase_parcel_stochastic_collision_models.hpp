// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_stochastic_collision_models_hpp_
#define make_reacting_multiphase_parcel_stochastic_collision_models_hpp_

#include "_no_stochastic_collision.hpp"
#include "_suppression_collision.hpp"

#define makeReactingMultiphaseParcelStochasticCollisionModels(CloudType)      \
                                                                              \
  makeStochasticCollisionModel(CloudType);                                    \
  makeStochasticCollisionModelType(NoStochasticCollision, CloudType);         \
  makeStochasticCollisionModelType(SuppressionCollision, CloudType);
#endif
