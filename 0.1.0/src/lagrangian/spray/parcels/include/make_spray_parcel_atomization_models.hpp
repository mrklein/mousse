// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_spray_parcel_atomization_models_hpp_
#define make_spray_parcel_atomization_models_hpp_

#include "_no_atomization.hpp"
#include "_blobs_sheet_atomization.hpp"
#include "_lisa_atomization.hpp"

#define makeSprayParcelAtomizationModels(CloudType)                           \
                                                                              \
  makeAtomizationModel(CloudType);                                            \
  makeAtomizationModelType(NoAtomization, CloudType);                         \
  makeAtomizationModelType(BlobsSheetAtomization, CloudType);                 \
  makeAtomizationModelType(LISAAtomization, CloudType);
#endif
