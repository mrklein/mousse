// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_injection_models_hpp_
#define make_parcel_injection_models_hpp_

#include "_cell_zone_injection.hpp"
#include "_cone_injection.hpp"
#include "_cone_nozzle_injection.hpp"
#include "_field_activated_injection.hpp"
#include "_inflation_injection.hpp"
#include "_kinematic_lookup_table_injection.hpp"
#include "_manual_injection.hpp"
#include "_no_injection.hpp"
#include "_patch_injection.hpp"
#include "_patch_flow_rate_injection.hpp"

#define MAKE_PARCEL_INJECTION_MODELS(CloudType)                               \
                                                                              \
  MAKE_INJECTION_MODEL(CloudType);                                            \
                                                                              \
  MAKE_INJECTION_MODEL_TYPE(CellZoneInjection, CloudType);                    \
  MAKE_INJECTION_MODEL_TYPE(ConeInjection, CloudType);                        \
  MAKE_INJECTION_MODEL_TYPE(ConeNozzleInjection, CloudType);                  \
  MAKE_INJECTION_MODEL_TYPE(FieldActivatedInjection, CloudType);              \
  MAKE_INJECTION_MODEL_TYPE(InflationInjection, CloudType);                   \
  MAKE_INJECTION_MODEL_TYPE(KinematicLookupTableInjection, CloudType);        \
  MAKE_INJECTION_MODEL_TYPE(ManualInjection, CloudType);                      \
  MAKE_INJECTION_MODEL_TYPE(NoInjection, CloudType);                          \
  MAKE_INJECTION_MODEL_TYPE(PatchInjection, CloudType);                       \
  MAKE_INJECTION_MODEL_TYPE(PatchFlowRateInjection, CloudType);

#endif
