// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_composition_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::CompositionModel<CloudType> >
mousse::CompositionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("compositionModel"));
  Info<< "Selecting composition model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "CompositionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown composition model type "
      << modelType << nl << nl
      << "Valid composition model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << nl
      << exit(FatalError);
  }
  return autoPtr<CompositionModel<CloudType> >(cstrIter()(dict, owner));
}
