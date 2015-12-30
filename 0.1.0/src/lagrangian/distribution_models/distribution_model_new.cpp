// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distribution_model.hpp"
// Selectors
mousse::autoPtr<mousse::distributionModels::distributionModel>
mousse::distributionModels::distributionModel::New
(
  const dictionary& dict,
  cachedRandom& rndGen
)
{
  const word modelType(dict.lookup("type"));
  Info<< "Selecting distribution model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "distributionModels::distributionModel::New"
      "("
        "const dictionary&, "
        "cachedRandom&"
      ")"
    )
      << "Unknown distribution model type " << modelType << nl << nl
      << "Valid distribution model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<distributionModel>(cstrIter()(dict, rndGen));
}
