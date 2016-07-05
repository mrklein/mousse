// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_change_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Selectors
autoPtr<phaseChangeModel> phaseChangeModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict
)
{
  word modelType{dict.lookup("phaseChangeModel")};
  Info << "    Selecting phaseChangeModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "phaseChangeModel::New(surfaceFilmModel&, const dictionary&)"
    )
    << "Unknown phaseChangeModel type " << modelType
    << nl << nl << "Valid phaseChangeModel types are:" << nl
    << dictionaryConstructorTablePtr_->toc()
    << exit(FatalError);
  }
  return autoPtr<phaseChangeModel>{cstrIter()(model, dict)};
}

} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace mousse

