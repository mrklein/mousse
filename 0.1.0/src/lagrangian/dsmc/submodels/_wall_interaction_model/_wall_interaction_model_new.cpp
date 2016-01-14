// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_interaction_model.hpp"

template<class CloudType>
mousse::autoPtr<mousse::WallInteractionModel<CloudType>>
mousse::WallInteractionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("WallInteractionModel"));
  Info<< "Selecting WallInteractionModel " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "WallInteractionModel<CloudType>::New"
      "(const dictionary&, CloudType&)"
    )
    << "Unknown WallInteractionModel type "
    << modelType << nl << nl
    << "Valid WallInteractionModel types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<WallInteractionModel<CloudType>>{cstrIter()(dict, owner)};
}
