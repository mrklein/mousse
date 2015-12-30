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

#define makeParcelInjectionModels(CloudType)                                  \
                                                                              \
  makeInjectionModel(CloudType);                                              \
                                                                              \
  makeInjectionModelType(CellZoneInjection, CloudType);                       \
  makeInjectionModelType(ConeInjection, CloudType);                           \
  makeInjectionModelType(ConeNozzleInjection, CloudType);                     \
  makeInjectionModelType(FieldActivatedInjection, CloudType);                 \
  makeInjectionModelType(InflationInjection, CloudType);                      \
  makeInjectionModelType(KinematicLookupTableInjection, CloudType);           \
  makeInjectionModelType(ManualInjection, CloudType);                         \
  makeInjectionModelType(NoInjection, CloudType);                             \
  makeInjectionModelType(PatchInjection, CloudType);                          \
  makeInjectionModelType(PatchFlowRateInjection, CloudType);

#endif
