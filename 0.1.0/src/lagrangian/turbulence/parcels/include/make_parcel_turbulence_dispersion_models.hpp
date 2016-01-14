// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_turbulence_dispersion_models_hpp_
#define make_parcel_turbulence_dispersion_models_hpp_

#include "_gradient_dispersion_ras.hpp"
#include "_stochastic_dispersion_ras.hpp"

#define MAKE_PARCEL_TURBULENCE_DISPERSION_MODELS(CloudType)                   \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::DispersionRASModel<kinematicCloudType>,                           \
    0                                                                         \
  );                                                                          \
                                                                              \
  MAKE_DISPERSION_MODEL_TYPE(GradientDispersionRAS, CloudType);               \
  MAKE_DISPERSION_MODEL_TYPE(StochasticDispersionRAS, CloudType);             \

#endif
