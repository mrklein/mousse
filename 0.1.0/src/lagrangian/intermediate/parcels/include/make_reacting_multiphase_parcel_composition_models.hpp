#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_COMPOSITION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_COMPOSITION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_composition.hpp"
#include "_single_mixture_fraction.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_COMPOSITION_MODELS(CloudType)         \
                                                                              \
  MAKE_COMPOSITION_MODEL(CloudType);                                          \
  MAKE_COMPOSITION_MODEL_TYPE(NoComposition, CloudType);                      \
  MAKE_COMPOSITION_MODEL_TYPE(SingleMixtureFraction, CloudType);

#endif
