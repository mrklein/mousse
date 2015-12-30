// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_heat_transfer_models_hpp_
#define make_parcel_heat_transfer_models_hpp_

#include "_no_heat_transfer.hpp"
#include "_ranz_marshall.hpp"

#define makeParcelHeatTransferModels(CloudType)                               \
                                                                              \
  makeHeatTransferModel(CloudType);                                           \
                                                                              \
  makeHeatTransferModelType(NoHeatTransfer, CloudType);                       \
  makeHeatTransferModelType(RanzMarshall, CloudType);

#endif
