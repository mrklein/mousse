// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_injection_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::InjectionModel<CloudType> >
mousse::InjectionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("injectionModel"));
  Info<< "Selecting injection model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "InjectionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown injection model type "
      << modelType << nl << nl
      << "Valid injection model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<InjectionModel<CloudType> >(cstrIter()(dict, owner));
}
template<class CloudType>
mousse::autoPtr<mousse::InjectionModel<CloudType> >
mousse::InjectionModel<CloudType>::New
(
  const dictionary& dict,
  const word& modelName,
  const word& modelType,
  CloudType& owner
)
{
  Info<< "Selecting injection model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "InjectionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "const word&, "
        "const word&, "
        "CloudType&"
      ")"
    )   << "Unknown injection model type "
      << modelType << nl << nl
      << "Valid injection model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return
    autoPtr<InjectionModel<CloudType> >
    (
      cstrIter()
      (
        dict,
        owner,
        modelName
      )
    );
}
