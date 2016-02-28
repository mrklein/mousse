// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_model.hpp"
#include "phase_system.hpp"
// Selector 
mousse::autoPtr<mousse::phaseModel> mousse::phaseModel::New
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
{
  word phaseModelType{fluid.subDict(phaseName).lookup("type")};
  Info << "Selecting phaseModel for " << phaseName << ": " << phaseModelType
    << endl;
  phaseSystemConstructorTable::iterator cstrIter =
    phaseSystemConstructorTablePtr_->find(phaseModelType);
  if (cstrIter == phaseSystemConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("phaseModel::New")
      << "Unknown phaseModelType type "
      << phaseModelType << endl << endl
      << "Valid phaseModel types are : " << endl
      << phaseSystemConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(fluid, phaseName, index);
}
