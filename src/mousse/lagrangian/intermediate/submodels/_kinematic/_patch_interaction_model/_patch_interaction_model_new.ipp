// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_patch_interaction_model.hpp"


template<class CloudType>
mousse::autoPtr<mousse::PatchInteractionModel<CloudType>>
mousse::PatchInteractionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("patchInteractionModel"));
  Info << "Selecting patch interaction model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "PatchInteractionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )
    << "Unknown patch interaction model type "
    << modelType << nl << nl
    << "Valid patch interaction model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<PatchInteractionModel<CloudType>>{cstrIter()(dict, owner)};
}

