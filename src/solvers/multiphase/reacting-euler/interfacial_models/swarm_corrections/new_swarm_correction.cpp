// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "swarm_correction.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::swarmCorrection>
mousse::swarmCorrection::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word swarmCorrectionType{dict.lookup("type")};
  Info << "Selecting swarmCorrection for "
    << pair << ": " << swarmCorrectionType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(swarmCorrectionType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("swarmCorrection::New")
      << "Unknown swarmCorrectionType type "
      << swarmCorrectionType << endl << endl
      << "Valid swarmCorrection types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
