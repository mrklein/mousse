// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_potential.hpp"
mousse::autoPtr<mousse::pairPotential> mousse::pairPotential::New
(
  const word& name,
  const dictionary& propDict
)
{
  const word potentialType(propDict.lookup("pairPotential"));
  Info<< nl << "Selecting intermolecular pair potential "
    << potentialType << " for "
    << name << " interaction." << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(potentialType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "pairPotential::New()"
    )   << "Unknown pairPotential type "
      << potentialType << nl << nl
      << "Valid pairPotentials are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<pairPotential>(cstrIter()(name, propDict));
}
