// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_devolatilisation_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::DevolatilisationModel<CloudType> >
mousse::DevolatilisationModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("devolatilisationModel"));
  Info<< "Selecting devolatilisation model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "DevolatilisationModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown devolatilisation model type "
      << modelType << nl << nl
      << "Valid devolatilisation model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<DevolatilisationModel<CloudType> >(cstrIter()(dict, owner));
}
