// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_parcel_phase_change_models_hpp_
#define make_reacting_parcel_phase_change_models_hpp_

#include "_no_phase_change.hpp"
#include "_liquid_evaporation.hpp"
#include "_liquid_evaporation_boil.hpp"

#define makeReactingParcelPhaseChangeModels(CloudType)                        \
                                                                              \
  makePhaseChangeModel(CloudType);                                            \
  makePhaseChangeModelType(NoPhaseChange, CloudType);                         \
  makePhaseChangeModelType(LiquidEvaporation, CloudType);                     \
  makePhaseChangeModelType(LiquidEvaporationBoil, CloudType);

#endif
