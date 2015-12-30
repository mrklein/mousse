// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_binary_collision_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::BinaryCollisionModel<CloudType> >
mousse::BinaryCollisionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("BinaryCollisionModel"));
  Info<< "Selecting BinaryCollisionModel " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "BinaryCollisionModel<CloudType>::New"
      "(const dictionary&, CloudType&)"
    )
      << "Unknown BinaryCollisionModel type "
      << modelType << nl << nl
      << "Valid BinaryCollisionModel types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<BinaryCollisionModel<CloudType> >
  (
    cstrIter()(dict, owner)
  );
}
