// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_mppic_parcel_isotropy_models_hpp_
#define make_mppic_parcel_isotropy_models_hpp_
#include "_no_isotropy.hpp"
#include "_stochastic.hpp"

#define makeMPPICParcelIsotropyModels(CloudType)                              \
                                                                              \
  makeIsotropyModel(CloudType);                                               \
                                                                              \
  makeIsotropyModelType(NoIsotropy, CloudType);                               \
  makeIsotropyModelType(Stochastic, CloudType);

#endif
