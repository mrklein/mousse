// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<filmViscosityModel> filmViscosityModel::New
(
  surfaceFilmModel& model,
  const dictionary& dict,
  volScalarField& mu
)
{
  word modelType(dict.lookup("filmViscosityModel"));
  Info<< "    Selecting filmViscosityModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "filmViscosityModel::New"
      "("
        "surfaceFilmModel&, "
        "const dictionary&, "
        "volScalarField&"
      ")"
    )   << "Unknown filmViscosityModel type " << modelType
      << nl << nl << "Valid filmViscosityModel types are:" << nl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<filmViscosityModel>(cstrIter()(model, dict, mu));
}
} // end namespace surfaceFilmModels
} // end namespace regionModels
} // end namespace mousse
