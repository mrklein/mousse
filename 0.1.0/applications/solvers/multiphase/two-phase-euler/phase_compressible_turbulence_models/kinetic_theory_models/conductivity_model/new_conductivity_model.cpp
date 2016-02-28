// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conductivity_model.hpp"
// Selectors
mousse::autoPtr<mousse::kineticTheoryModels::conductivityModel>
mousse::kineticTheoryModels::conductivityModel::New
(
  const dictionary& dict
)
{
  word conductivityModelType(dict.lookup("conductivityModel"));
  Info<< "Selecting conductivityModel "
    << conductivityModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(conductivityModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalError
      << "conductivityModel::New(const dictionary&) : " << endl
      << "    unknown conductivityModelType type "
      << conductivityModelType
      << ", constructor not in hash table" << endl << endl
      << "    Valid conductivityModelType types are :" << endl;
    Info<< dictionaryConstructorTablePtr_->sortedToc() << abort(FatalError);
  }
  return autoPtr<conductivityModel>(cstrIter()(dict));
}
