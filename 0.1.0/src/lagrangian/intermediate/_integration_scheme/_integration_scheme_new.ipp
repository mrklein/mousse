// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "_integration_scheme.hpp"


// Constructors 
template<class Type>
mousse::autoPtr<mousse::IntegrationScheme<Type>>
mousse::IntegrationScheme<Type>::New
(
  const word& phiName,
  const dictionary& dict
)
{
  const word schemeName{dict.lookup(phiName)};
  Info << "Selecting " << phiName << " integration scheme "
    << schemeName << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(schemeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "IntegrationScheme::New(const dictionary&)"
    )
    << "Unknown integration scheme type "
    << schemeName << nl << nl
    << "Valid integration scheme types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc() << nl
    << exit(FatalError);
  }
  return autoPtr<IntegrationScheme<Type>>{cstrIter()(phiName, dict)};
}

