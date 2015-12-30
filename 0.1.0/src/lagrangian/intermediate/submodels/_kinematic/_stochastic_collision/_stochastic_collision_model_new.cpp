// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_stochastic_collision_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::StochasticCollisionModel<CloudType> >
mousse::StochasticCollisionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word modelType(dict.lookup("stochasticCollisionModel"));
  Info<< "Selecting stochastic collision model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "StochasticCollisionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown model type type "
      << modelType << ", constructor not in hash table" << nl << nl
      << "    Valid model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<StochasticCollisionModel<CloudType> >
  (
    cstrIter()(dict, owner)
  );
}
