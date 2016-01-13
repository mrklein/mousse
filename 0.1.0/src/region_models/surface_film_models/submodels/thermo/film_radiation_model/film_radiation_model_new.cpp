// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_radiation_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<filmRadiationModel> filmRadiationModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict
)
{
  word modelType(dict.lookup("radiationModel"));
  Info<< "    Selecting radiationModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "filmRadiationModel::New"
      "("
        "surfaceFilmModel&, "
        "const dictionary&"
      ")"
    )   << "Unknown radiationModel type " << modelType << nl << nl
      << "Valid filmRadiationModel types are:" << nl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<filmRadiationModel>(cstrIter()(model, dict));
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
