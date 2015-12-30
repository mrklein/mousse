// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_phase_change_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::PhaseChangeModel<CloudType> >
mousse::PhaseChangeModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("phaseChangeModel"));
  Info<< "Selecting phase change model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "PhaseChangeModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown phase change model type "
      << modelType << nl << nl
      << "Valid phase change model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<PhaseChangeModel<CloudType> >(cstrIter()(dict, owner));
}
