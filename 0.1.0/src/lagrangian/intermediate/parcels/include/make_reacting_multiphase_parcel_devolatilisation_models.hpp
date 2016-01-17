// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_devolatilisation_models_hpp_
#define make_reacting_multiphase_parcel_devolatilisation_models_hpp_

#include "_constant_rate_devolatilisation.hpp"
#include "_no_devolatilisation.hpp"
#include "_single_kinetic_rate_devolatilisation.hpp"

#define MAKE_REACTING_MULTIPHASE_PARCEL_DEVOLATILISATION_MODELS(CloudType)    \
                                                                              \
  MAKE_DEVOLATILISATION_MODEL(CloudType);                                     \
  MAKE_DEVOLATILISATION_MODEL_TYPE(ConstantRateDevolatilisation, CloudType);  \
  MAKE_DEVOLATILISATION_MODEL_TYPE(NoDevolatilisation, CloudType);            \
  MAKE_DEVOLATILISATION_MODEL_TYPE                                            \
  (                                                                           \
    SingleKineticRateDevolatilisation,                                        \
    CloudType                                                                 \
  );
#endif
