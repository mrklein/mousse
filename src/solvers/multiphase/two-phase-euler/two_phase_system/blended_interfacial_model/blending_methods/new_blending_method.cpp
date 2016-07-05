// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_method.hpp"
mousse::autoPtr<mousse::blendingMethod> mousse::blendingMethod::New
(
  const dictionary& dict,
  const wordList& phaseNames
)
{
  word blendingMethodType{dict.lookup("type")};
  Info << "Selecting " << dict.dictName() << " blending method: "
    << blendingMethodType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(blendingMethodType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("blendingMethod::New")
      << "Unknown blendingMethodType type "
      << blendingMethodType << endl << endl
      << "Valid blendingMethod types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, phaseNames);
}
