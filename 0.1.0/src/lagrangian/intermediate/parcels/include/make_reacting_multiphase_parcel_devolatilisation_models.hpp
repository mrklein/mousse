// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_multiphase_parcel_devolatilisation_models_hpp_
#define make_reacting_multiphase_parcel_devolatilisation_models_hpp_

#include "_constant_rate_devolatilisation.hpp"
#include "_no_devolatilisation.hpp"
#include "_single_kinetic_rate_devolatilisation.hpp"

#define makeReactingMultiphaseParcelDevolatilisationModels(CloudType)         \
                                                                              \
  makeDevolatilisationModel(CloudType);                                       \
  makeDevolatilisationModelType(ConstantRateDevolatilisation, CloudType);     \
  makeDevolatilisationModelType(NoDevolatilisation, CloudType);               \
  makeDevolatilisationModelType                                               \
  (                                                                           \
    SingleKineticRateDevolatilisation,                                        \
    CloudType                                                                 \
  );
#endif
