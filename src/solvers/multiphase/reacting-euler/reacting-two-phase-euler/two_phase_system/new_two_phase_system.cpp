// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "two_phase_system.hpp"
// Selector 
mousse::autoPtr<mousse::twoPhaseSystem> mousse::twoPhaseSystem::New
(
  const fvMesh& mesh
)
{
  const word twoPhaseSystemType
  {
    IOdictionary
    (
      {
        propertiesName,
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    ).lookup("type")
  };
  Info << "Selecting twoPhaseSystem "
    << twoPhaseSystemType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(twoPhaseSystemType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("twoPhaseSystem::New")
      << "Unknown twoPhaseSystemType type "
      << twoPhaseSystemType << endl << endl
      << "Valid twoPhaseSystem types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(mesh);
}
