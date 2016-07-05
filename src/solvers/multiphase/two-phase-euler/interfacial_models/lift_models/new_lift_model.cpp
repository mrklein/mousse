// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lift_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::liftModel> mousse::liftModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word liftModelType{dict.lookup("type")};
  Info << "Selecting liftModel for "
    << pair << ": " << liftModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(liftModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("liftModel::New")
      << "Unknown liftModelType type "
      << liftModelType << endl << endl
      << "Valid liftModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
