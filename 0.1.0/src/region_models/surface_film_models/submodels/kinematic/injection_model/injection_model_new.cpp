// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "injection_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<injectionModel> injectionModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict,
  const word& modelType
)
{
  Info<< "        " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "injectionModel::New"
      "("
        "surfaceFilmModel&, "
        "const dictionary&, "
        "const word&"
      ")"
    )   << "Unknown injectionModel type " << modelType
      << nl << nl << "Valid injectionModel types are:" << nl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<injectionModel>(cstrIter()(model, dict));
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
