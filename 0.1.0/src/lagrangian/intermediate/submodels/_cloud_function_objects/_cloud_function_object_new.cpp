// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud_function_object.hpp"
template<class CloudType>
mousse::autoPtr<mousse::CloudFunctionObject<CloudType> >
mousse::CloudFunctionObject<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner,
  const word& objectType,
  const word& modelName
)
{
  Info<< "    Selecting cloud function " << modelName << " of type "
    << objectType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(objectType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "CloudFunctionObject<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&, "
        "const word&, "
        "const word&"
      ")"
    )   << "Unknown cloud function type "
      << objectType << nl << nl
      << "Valid cloud function types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<CloudFunctionObject<CloudType> >
  (
    cstrIter()
    (
      dict,
      owner,
      modelName
    )
  );
}
