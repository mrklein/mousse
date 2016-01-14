// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_mppic_parcel_isotropy_models_hpp_
#define make_mppic_parcel_isotropy_models_hpp_
#include "_no_isotropy.hpp"
#include "_stochastic.hpp"

#define MAKE_MPPIC_PARCEL_ISOTROPY_MODELS(CloudType)                          \
                                                                              \
  MAKE_ISOTROPY_MODEL(CloudType);                                             \
                                                                              \
  MAKE_ISOTROPY_MODEL_TYPE(NoIsotropy, CloudType);                            \
  MAKE_ISOTROPY_MODEL_TYPE(Stochastic, CloudType);

#endif
