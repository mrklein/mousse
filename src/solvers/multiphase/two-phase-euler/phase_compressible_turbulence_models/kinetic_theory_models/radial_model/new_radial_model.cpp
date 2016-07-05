// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radial_model.hpp"
// Selectors
mousse::autoPtr<mousse::kineticTheoryModels::radialModel>
mousse::kineticTheoryModels::radialModel::New
(
  const dictionary& dict
)
{
  word radialModelType(dict.lookup("radialModel"));
  Info<< "Selecting radialModel "
    << radialModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(radialModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalError
      << "radialModel::New(const dictionary&) : " << endl
      << "    unknown radialModelType type "
      << radialModelType
      << ", constructor not in hash table" << endl << endl
      << "    Valid radialModelType types are :" << endl;
    Info<< dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<radialModel>(cstrIter()(dict));
}
