// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Selectors
autoPtr<heatTransferModel> heatTransferModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict
)
{
  word modelType{dict.lookup("heatTransferModel")};
  Info << "    Selecting heatTransferModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "heatTransferModel::New(surfaceFilmModel&, const dictionary&)"
    )
    << "Unknown heatTransferModel type " << modelType << nl << nl
    << "Valid heatTransferModel types are:" << nl
    << dictionaryConstructorTablePtr_->toc()
    << exit(FatalError);
  }
  return autoPtr<heatTransferModel>{cstrIter()(model, dict)};
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

