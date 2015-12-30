// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_dispersion_models_hpp_
#define make_parcel_dispersion_models_hpp_

#include "_no_dispersion.hpp"
#define makeParcelDispersionModels(CloudType)                                 \
                                                                              \
  makeDispersionModel(CloudType);                                             \
  makeDispersionModelType(NoDispersion, CloudType);

#endif
