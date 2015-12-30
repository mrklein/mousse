// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_spray_parcel_breakup_models_hpp_
#define make_spray_parcel_breakup_models_hpp_

#include "_no_breakup.hpp"
#include "_pilch_erdman.hpp"
#include "_reitz_diwakar.hpp"
#include "_reitz_khrt.hpp"
#include "_tab.hpp"
#include "_etab.hpp"
#include "_shf.hpp"

#define makeSprayParcelBreakupModels(CloudType)                               \
                                                                              \
  makeBreakupModel(CloudType)                                                 \
  makeBreakupModelType(NoBreakup, CloudType);                                 \
  makeBreakupModelType(PilchErdman, CloudType);                               \
  makeBreakupModelType(ReitzDiwakar, CloudType);                              \
  makeBreakupModelType(ReitzKHRT, CloudType);                                 \
  makeBreakupModelType(TAB, CloudType);                                       \
  makeBreakupModelType(ETAB, CloudType);                                      \
  makeBreakupModelType(SHF, CloudType);

#endif
