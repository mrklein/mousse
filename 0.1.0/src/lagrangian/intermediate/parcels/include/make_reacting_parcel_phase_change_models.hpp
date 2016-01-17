// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_parcel_phase_change_models_hpp_
#define make_reacting_parcel_phase_change_models_hpp_

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
