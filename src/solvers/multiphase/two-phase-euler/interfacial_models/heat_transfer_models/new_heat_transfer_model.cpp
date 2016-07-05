// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::heatTransferModel> mousse::heatTransferModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word heatTransferModelType{dict.lookup("type")};
  Info << "Selecting heatTransferModel for "
    << pair << ": " << heatTransferModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(heatTransferModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("heatTransferModel::New")
      << "Unknown heatTransferModelType type "
      << heatTransferModelType << endl << endl
      << "Valid heatTransferModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
