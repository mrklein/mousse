#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_PARCEL_PHASE_CHANGE_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_PARCEL_PHASE_CHANGE_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "_no_phase_change.hpp"
#include "_liquid_evaporation.hpp"
#include "_liquid_evaporation_boil.hpp"

#define MAKE_REACTING_PARCEL_PHASE_CHANGE_MODELS(CloudType)                   \
                                                                              \
  MAKE_PHASE_CHANGE_MODEL(CloudType);                                         \
  MAKE_PHASE_CHANGE_MODEL_TYPE(NoPhaseChange, CloudType);                     \
  MAKE_PHASE_CHANGE_MODEL_TYPE(LiquidEvaporation, CloudType);                 \
  MAKE_PHASE_CHANGE_MODEL_TYPE(LiquidEvaporationBoil, CloudType);

#endif
