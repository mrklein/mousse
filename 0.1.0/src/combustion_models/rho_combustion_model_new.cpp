// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_combustion_model.hpp"
// Selectors
mousse::autoPtr<mousse::combustionModels::rhoCombustionModel>
mousse::combustionModels::rhoCombustionModel::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  const word combTypeName
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
  Info<< "Selecting combustion model " << combTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(combTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "rhoCombustionModel::New"
    )   << "Unknown rhoCombustionModel type "
      << combTypeName << endl << endl
      << "Valid  combustionModels are : " << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  const label tempOpen = combTypeName.find('<');
  const word className = combTypeName(0, tempOpen);
  return autoPtr<rhoCombustionModel> (cstrIter()(className, mesh, phaseName));
}
