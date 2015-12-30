// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_combustion_model.hpp"
// Selectors
mousse::autoPtr<mousse::combustionModels::psiCombustionModel>
mousse::combustionModels::psiCombustionModel::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  const word combModelName
  (
    IOdictionary
    (
      IOobject
      (
        IOobject::groupName("combustionProperties", phaseName),
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      )
    ).lookup("combustionModel")
  );
  Info<< "Selecting combustion model " << combModelName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(combModelName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "psiCombustionModel::New"
    )   << "Unknown psiCombustionModel type "
      << combModelName << endl << endl
      << "Valid  combustionModels are : " << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  const label tempOpen = combModelName.find('<');
  const word className = combModelName(0, tempOpen);
  return autoPtr<psiCombustionModel>(cstrIter()(className, mesh, phaseName));
}
