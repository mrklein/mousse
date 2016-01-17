// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laminar_flame_speed.hpp"
mousse::autoPtr<mousse::laminarFlameSpeed> mousse::laminarFlameSpeed::New
(
  const psiuReactionThermo& ct
)
{
  // do not register the dictionary
  IOdictionary propDict
  (
    IOobject
    (
      "combustionProperties",
      ct.T().time().constant(),
      ct.T().db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    )
  );
  const word corrType(propDict.lookup("laminarFlameSpeedCorrelation"));
  Info<< "Selecting laminar flame speed correlation " << corrType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(corrType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "laminarFlameSpeed::New(const psiuReactionThermo&)",
      propDict
    )   << "Unknown laminarFlameSpeed type "
      << corrType << nl << nl
      << "Valid laminarFlameSpeed types are :" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalIOError);
  }
  return autoPtr<laminarFlameSpeed>(cstrIter()(propDict, ct));
}
