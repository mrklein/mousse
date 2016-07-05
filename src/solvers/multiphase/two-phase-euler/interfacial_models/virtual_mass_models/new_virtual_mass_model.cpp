// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "virtual_mass_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::virtualMassModel> mousse::virtualMassModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word virtualMassModelType{dict.lookup("type")};
  Info<< "Selecting virtualMassModel for "
    << pair << ": " << virtualMassModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(virtualMassModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("virtualMassModel::New")
      << "Unknown virtualMassModelType type "
      << virtualMassModelType << endl << endl
      << "Valid virtualMassModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair, true);
}
