// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diameter_model.hpp"
mousse::autoPtr<mousse::diameterModel> mousse::diameterModel::New
(
  const dictionary& dict,
  const phaseModel& phase
)
{
  word diameterModelType{dict.lookup("diameterModel")};
  Info << "Selecting diameterModel for phase "
    << phase.name()
    << ": "
    << diameterModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(diameterModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("diameterModel::New")
     << "Unknown diameterModelType type "
     << diameterModelType << endl << endl
     << "Valid diameterModel types are : " << endl
     << dictionaryConstructorTablePtr_->sortedToc()
     << exit(FatalError);
  }
  return cstrIter()(dict.subDict(diameterModelType + "Coeffs"), phase);
}
