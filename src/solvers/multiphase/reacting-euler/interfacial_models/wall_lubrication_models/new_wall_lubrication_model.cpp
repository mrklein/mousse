// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_lubrication_model.hpp"
#include "phase_pair.hpp"
// Selector 
mousse::autoPtr<mousse::wallLubricationModel> mousse::wallLubricationModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word wallLubricationModelType{dict.lookup("type")};
  Info << "Selecting wallLubricationModel for "
    << pair << ": " << wallLubricationModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(wallLubricationModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("wallLubricationModel::New")
      << "Unknown wallLubricationModelType type "
      << wallLubricationModelType << endl << endl
      << "Valid wallLubricationModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
