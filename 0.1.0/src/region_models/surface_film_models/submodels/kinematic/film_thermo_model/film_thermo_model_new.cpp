// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_thermo_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Selectors
autoPtr<filmThermoModel> filmThermoModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict
)
{
  word modelType{dict.lookup("filmThermoModel")};
  Info << "    Selecting filmThermoModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "filmThermoModel::New(surfaceFilmModel&, const dictionary&)"
    )
    << "Unknown filmThermoModel type " << modelType << nl << nl
    << "Valid filmThermoModel types are:" << nl
    << dictionaryConstructorTablePtr_->toc()
    << exit(FatalError);
  }
  return autoPtr<filmThermoModel>{cstrIter()(model, dict)};
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

