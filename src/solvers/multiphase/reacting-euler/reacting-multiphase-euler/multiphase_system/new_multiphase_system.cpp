// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multiphase_system.hpp"
// Selector 
mousse::autoPtr<mousse::multiphaseSystem> mousse::multiphaseSystem::New
(
  const fvMesh& mesh
)
{
  const word multiphaseSystemType
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
  Info<< "Selecting multiphaseSystem " << multiphaseSystemType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(multiphaseSystemType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("multiphaseSystem::New")
      << "Unknown multiphaseSystemType type "
      << multiphaseSystemType << endl << endl
      << "Valid multiphaseSystem types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(mesh);
}
