// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_surface_film_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::SurfaceFilmModel<CloudType> >
mousse::SurfaceFilmModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("surfaceFilmModel"));
  Info<< "Selecting surface film model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "SurfaceFilmModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown surface film model type "
      << modelType << nl << nl
      << "Valid surface film model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<SurfaceFilmModel<CloudType> >(cstrIter()(dict, owner));
}
