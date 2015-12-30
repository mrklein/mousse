// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_mppic_parcel_packing_models_hpp_
#define make_mppic_parcel_packing_models_hpp_
#include "_no_packing.hpp"
#include "_explicit.hpp"
#include "_implicit.hpp"

#define makeMPPICParcelPackingModels(CloudType)                               \
                                                                              \
  makePackingModel(CloudType);                                                \
                                                                              \
  makePackingModelType(NoPacking, CloudType);                                 \
  makePackingModelType(Explicit, CloudType);                                  \
  makePackingModelType(Implicit, CloudType);

#endif
