// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "aspect_ratio_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::aspectRatioModel>
mousse::aspectRatioModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word aspectRatioModelType{dict.lookup("type")};
  Info<< "Selecting aspectRatioModel for "
    << pair << ": " << aspectRatioModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(aspectRatioModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("aspectRatioModel::New")
      << "Unknown aspectRatioModelType type "
      << aspectRatioModelType << endl << endl
      << "Valid aspectRatioModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
