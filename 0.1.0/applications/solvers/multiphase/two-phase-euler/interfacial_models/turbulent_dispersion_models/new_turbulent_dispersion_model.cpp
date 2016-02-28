// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_dispersion_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::turbulentDispersionModel>
mousse::turbulentDispersionModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word turbulentDispersionModelType{dict.lookup("type")};
  Info << "Selecting turbulentDispersionModel for "
    << pair << ": " << turbulentDispersionModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(turbulentDispersionModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("turbulentDispersionModel::New")
      << "Unknown turbulentDispersionModelType type "
      << turbulentDispersionModelType << endl << endl
      << "Valid turbulentDispersionModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
