// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "saturation_model.hpp"
// Selector 
mousse::autoPtr<mousse::saturationModel> mousse::saturationModel::New
(
  const dictionary& dict
)
{
  word saturationModelType{dict.lookup("type")};
  Info << "Selecting saturationModel: "
    << saturationModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(saturationModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("saturationModel::New")
      << "Unknown saturationModelType type "
      << saturationModelType << endl << endl
      << "Valid saturationModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict);
}
