// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "drag_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::dragModel> mousse::dragModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word dragModelType{dict.lookup("type")};
  Info<< "Selecting dragModel for "
    << pair << ": " << dragModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(dragModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("dragModel::New")
      << "Unknown dragModelType type "
      << dragModelType << endl << endl
      << "Valid dragModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair, true);
}
