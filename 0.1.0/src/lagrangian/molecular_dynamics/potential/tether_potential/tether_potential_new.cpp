// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tether_potential.hpp"
mousse::autoPtr<mousse::tetherPotential> mousse::tetherPotential::New
(
  const word& name,
  const dictionary& propDict
)
{
  const word potentialType(propDict.lookup("tetherPotential"));
  Info<< nl << "Selecting tether potential " << potentialType
    << " for " << name << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(potentialType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "tetherPotential::New()"
    )   << "Unknown tetherPotential type "
      << potentialType << nl << nl
      << "Valid tetherPotentials are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<tetherPotential>(cstrIter()(name, propDict));
}
