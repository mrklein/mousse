// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "help_type.hpp"
mousse::autoPtr<mousse::helpType> mousse::helpType::New
(
  const word& helpTypeName
)
{
  Info<< "Selecting helpType " << helpTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(helpTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    // special treatment for -help
    // exit without stack trace
    if (helpTypeName == "-help")
    {
      FatalErrorIn("helpType::New()")
        << "Valid helpType selections are:" << nl
        << dictionaryConstructorTablePtr_->sortedToc() << nl
        << exit(FatalError);
    }
    else
    {
      FatalErrorIn("helpType::New()")
        << "Unknown helpType type " << helpTypeName << nl
        << "Valid helpType selections are:" << nl
        << dictionaryConstructorTablePtr_->sortedToc() << nl
        << abort(FatalError);
    }
  }
  return autoPtr<helpType>(cstrIter()());
}
