// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "heat_transfer_model.hpp"
mousse::autoPtr<mousse::heatTransferModel> mousse::heatTransferModel::New
(
  const dictionary& interfaceDict,
  const volScalarField& alpha1,
  const phaseModel& phase1,
  const phaseModel& phase2
)
{
  word heatTransferModelType
  {
    interfaceDict.lookup("heatTransferModel" + phase1.name())
  };
  Info << "Selecting heatTransferModel for phase "
    << phase1.name()
    << ": "
    << heatTransferModelType << endl;
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
  return cstrIter()(interfaceDict, alpha1, phase1, phase2);
}
