// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_collision_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::CollisionModel<CloudType> >
mousse::CollisionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word modelType(dict.lookup("collisionModel"));
  Info<< "Selecting collision model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "CollisionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown collision model type " << modelType
      << ", constructor not in hash table" << nl << nl
      << "    Valid collision model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<CollisionModel<CloudType> >(cstrIter()(dict, owner));
}
