#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_INJECTION_MODELS_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_INCLUDE_MAKE_REACTING_MULTIPHASE_PARCEL_INJECTION_MODELS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cell_zone_injection.hpp"
#include "_cone_injection.hpp"
#include "_cone_nozzle_injection.hpp"
#include "_field_activated_injection.hpp"
#include "_manual_injection.hpp"
#include "_no_injection.hpp"
#include "_patch_injection.hpp"
#include "_patch_flow_rate_injection.hpp"
#include "_reacting_multiphase_lookup_table_injection.hpp"


#define MAKE_REACTING_MULTIPHASE_PARCEL_INJECTION_MODELS(CloudType)           \
                                                                              \
  MAKE_INJECTION_MODEL(CloudType);                                            \
  MAKE_INJECTION_MODEL_TYPE(CellZoneInjection, CloudType);                    \
  MAKE_INJECTION_MODEL_TYPE(ConeInjection, CloudType);                        \
  MAKE_INJECTION_MODEL_TYPE(ConeNozzleInjection, CloudType);                  \
  MAKE_INJECTION_MODEL_TYPE(FieldActivatedInjection, CloudType);              \
  MAKE_INJECTION_MODEL_TYPE(ManualInjection, CloudType);                      \
  MAKE_INJECTION_MODEL_TYPE(NoInjection, CloudType);                          \
  MAKE_INJECTION_MODEL_TYPE(PatchInjection, CloudType);                       \
  MAKE_INJECTION_MODEL_TYPE(PatchFlowRateInjection, CloudType);               \
  MAKE_INJECTION_MODEL_TYPE(ReactingMultiphaseLookupTableInjection, CloudType);

#endif

