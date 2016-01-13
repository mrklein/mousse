// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "force.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<force> force::New
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
      "force::New"
      "("
        "const surfaceFilmModel&, "
        "const dictionary&, "
        "const word&"
      ")"
    )   << "Unknown force type " << modelType
      << nl << nl << "Valid force types are:" << nl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<force>(cstrIter()(model, dict));
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
