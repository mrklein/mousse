// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_surface_reaction_model.hpp"


template<class CloudType>
mousse::autoPtr<mousse::SurfaceReactionModel<CloudType> >
mousse::SurfaceReactionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType{dict.lookup("surfaceReactionModel")};
  Info << "Selecting surface reaction model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "SurfaceReactionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )
    << "Unknown surface reaction model type "
    << modelType << nl << nl
    << "Valid surface reaction model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<SurfaceReactionModel<CloudType>>{cstrIter()(dict, owner)};
}
