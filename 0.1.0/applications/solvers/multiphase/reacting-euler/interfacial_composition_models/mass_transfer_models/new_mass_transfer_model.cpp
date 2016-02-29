// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mass_transfer_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::massTransferModel> mousse::massTransferModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word massTransferModelType{dict.lookup("type")};
  Info << "Selecting massTransferModel for "
    << pair << ": " << massTransferModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(massTransferModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("massTransferModel::New")
      << "Unknown massTransferModelType type "
      << massTransferModelType << endl << endl
      << "Valid massTransferModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
