// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "energy_scaling_function.hpp"


mousse::autoPtr<mousse::energyScalingFunction> mousse::energyScalingFunction::New
(
  const word& name,
  const dictionary& propDict,
  const pairPotential& pairPot
)
{
  const word scalingType{propDict.lookup("energyScalingFunction")};
  Info << "Selecting energy scaling function "
    << scalingType << " for "
    << name << " potential energy." << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(scalingType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "energyScalingFunction::New()"
    )
    << "Unknown energyScalingFunction type "
    << scalingType << nl << nl
    << "Valid energyScalingFunctions are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<energyScalingFunction>{cstrIter()(name, propDict, pairPot)};
}

