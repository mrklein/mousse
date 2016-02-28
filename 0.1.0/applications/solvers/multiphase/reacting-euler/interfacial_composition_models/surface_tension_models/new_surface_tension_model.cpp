// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_tension_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::surfaceTensionModel >
mousse::surfaceTensionModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word surfaceTensionModelType{dict.lookup("type")};
  Info << "Selecting surfaceTensionModel for "
    << pair << ": " << surfaceTensionModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(surfaceTensionModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("surfaceTensionModel::New")
      << "Unknown surfaceTensionModelType type "
      << surfaceTensionModelType << endl << endl
      << "Valid surfaceTensionModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair, true);
}
