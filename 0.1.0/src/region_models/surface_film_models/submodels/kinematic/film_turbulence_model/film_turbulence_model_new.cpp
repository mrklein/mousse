// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_turbulence_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<filmTurbulenceModel> filmTurbulenceModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict
)
{
  const word modelType(dict.lookup("turbulence"));
  Info<< "    Selecting filmTurbulenceModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "filmTurbulenceModel::New"
      "("
        "surfaceFilmModel&, "
        "const dictionary&"
      ")"
    )   << "Unknown filmTurbulenceModel type " << modelType
      << nl << nl << "Valid filmTurbulenceModel types are:" << nl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<filmTurbulenceModel>(cstrIter()(model, dict));
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
