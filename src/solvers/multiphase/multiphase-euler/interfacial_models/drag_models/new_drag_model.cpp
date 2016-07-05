// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "drag_model.hpp"
mousse::autoPtr<mousse::dragModel> mousse::dragModel::New
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
{
  word dragModelType{interfaceDict.lookup("type")};
  Info << "Selecting dragModel for phase "
    << phase1.name()
    << ": "
    << dragModelType << endl;
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
  return cstrIter()(interfaceDict, phase1, phase2);
}
