// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_parcel_turbulence_dispersion_models_hpp_
#define make_parcel_turbulence_dispersion_models_hpp_

#include "_gradient_dispersion_ras.hpp"
#include "_stochastic_dispersion_ras.hpp"

#define makeParcelTurbulenceDispersionModels(CloudType)                       \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::DispersionRASModel<kinematicCloudType>,                           \
    0                                                                         \
  );                                                                          \
                                                                              \
  makeDispersionModelType(GradientDispersionRAS, CloudType);                  \
  makeDispersionModelType(StochasticDispersionRAS, CloudType);                \

#endif
