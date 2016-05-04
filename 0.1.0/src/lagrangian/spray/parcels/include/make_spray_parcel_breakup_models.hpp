#ifndef LAGRANGIAN_SPRAY_PARCELS_INCLUDE_MAKE_SPRAY_PARCEL_BREAKUP_MODELS_HPP_
#define LAGRANGIAN_SPRAY_PARCELS_INCLUDE_MAKE_SPRAY_PARCEL_BREAKUP_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_breakup.hpp"
#include "_pilch_erdman.hpp"
#include "_reitz_diwakar.hpp"
#include "_reitz_khrt.hpp"
#include "_tab.hpp"
#include "_etab.hpp"
#include "_shf.hpp"


#define MAKE_SPRAY_PARCEL_BREAKUP_MODELS(CloudType)                           \
                                                                              \
  MAKE_BREAKUP_MODEL(CloudType)                                               \
  MAKE_BREAKUP_MODEL_TYPE(NoBreakup, CloudType);                              \
  MAKE_BREAKUP_MODEL_TYPE(PilchErdman, CloudType);                            \
  MAKE_BREAKUP_MODEL_TYPE(ReitzDiwakar, CloudType);                           \
  MAKE_BREAKUP_MODEL_TYPE(ReitzKHRT, CloudType);                              \
  MAKE_BREAKUP_MODEL_TYPE(TAB, CloudType);                                    \
  MAKE_BREAKUP_MODEL_TYPE(ETAB, CloudType);                                   \
  MAKE_BREAKUP_MODEL_TYPE(SHF, CloudType);

#endif

