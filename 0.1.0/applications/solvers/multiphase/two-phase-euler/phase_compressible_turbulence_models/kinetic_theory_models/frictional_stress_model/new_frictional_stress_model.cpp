// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "frictional_stress_model.hpp"
// Selectors
mousse::autoPtr<mousse::kineticTheoryModels::frictionalStressModel>
mousse::kineticTheoryModels::frictionalStressModel::New
(
  const dictionary& dict
)
{
  word frictionalStressModelType(dict.lookup("frictionalStressModel"));
  Info<< "Selecting frictionalStressModel "
    << frictionalStressModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(frictionalStressModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalError
      << "frictionalStressModel::New(const dictionary&) : " << endl
      << "    unknown frictionalStressModelType type "
      << frictionalStressModelType
      << ", constructor not in hash table" << endl << endl
      << "    Valid frictionalStressModelType types are :" << endl;
    Info<< dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<frictionalStressModel>(cstrIter()(dict));
}
