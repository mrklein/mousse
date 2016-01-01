// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_type.hpp"
mousse::autoPtr<mousse::calcType> mousse::calcType::New
(
  const word& calcTypeName
)
{
  Info<< "Selecting calcType " << calcTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(calcTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    // special treatment for -help
    // exit without stack trace
    if (calcTypeName == "-help")
    {
      FatalErrorIn("calcType::New()")
        << "Valid calcType selections are:" << nl
        << dictionaryConstructorTablePtr_->sortedToc() << nl
        << exit(FatalError);
    }
    else
    {
      FatalErrorIn("calcType::New()")
        << "Unknown calcType type " << calcTypeName << nl
        << "Valid calcType selections are:" << nl
        << dictionaryConstructorTablePtr_->sortedToc() << nl
        << abort(FatalError);
    }
  }
  return autoPtr<calcType>(cstrIter()());
}
