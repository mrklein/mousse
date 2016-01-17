// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_heat_transfer_models_hpp_
#define make_parcel_heat_transfer_models_hpp_

#include "_no_heat_transfer.hpp"
#include "_ranz_marshall.hpp"

#define MAKE_PARCEL_HEAT_TRANSFER_MODELS(CloudType)                           \
                                                                              \
  MAKE_HEAT_TRANSFER_MODEL(CloudType);                                        \
                                                                              \
  MAKE_HEAT_TRANSFER_MODEL_TYPE(NoHeatTransfer, CloudType);                   \
  MAKE_HEAT_TRANSFER_MODEL_TYPE(RanzMarshall, CloudType);

#endif
