// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_composition_models_hpp_
#define make_reacting_multiphase_parcel_composition_models_hpp_

#include "_no_composition.hpp"
#include "_single_mixture_fraction.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_COMPOSITION_MODELS(CloudType)         \
                                                                              \
  MAKE_COMPOSITION_MODEL(CloudType);                                          \
  MAKE_COMPOSITION_MODEL_TYPE(NoComposition, CloudType);                      \
  MAKE_COMPOSITION_MODEL_TYPE(SingleMixtureFraction, CloudType);

#endif
