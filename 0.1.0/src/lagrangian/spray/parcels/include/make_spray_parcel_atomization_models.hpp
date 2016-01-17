// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_spray_parcel_atomization_models_hpp_
#define make_spray_parcel_atomization_models_hpp_

#include "_no_atomization.hpp"
#include "_blobs_sheet_atomization.hpp"
#include "_lisa_atomization.hpp"

#define MAKE_SPRAY_PARCEL_ATOMIZATION_MODELS(CloudType)                       \
                                                                              \
  MAKE_ATOMIZATION_MODEL(CloudType);                                          \
  MAKE_ATOMIZATION_MODEL_TYPE(NoAtomization, CloudType);                      \
  MAKE_ATOMIZATION_MODEL_TYPE(BlobsSheetAtomization, CloudType);              \
  MAKE_ATOMIZATION_MODEL_TYPE(LISAAtomization, CloudType);
#endif
