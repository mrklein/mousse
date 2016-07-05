// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "granular_pressure_model.hpp"
// Selectors
mousse::autoPtr<mousse::kineticTheoryModels::granularPressureModel>
mousse::kineticTheoryModels::granularPressureModel::New
(
  const dictionary& dict
)
{
  word granularPressureModelType(dict.lookup("granularPressureModel"));
  Info<< "Selecting granularPressureModel "
    << granularPressureModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(granularPressureModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalError
      << "granularPressureModel::New(const dictionary&) : " << endl
      << "    unknown granularPressureModelType type "
      << granularPressureModelType
      << ", constructor not in hash table" << endl << endl
      << "    Valid granularPressureModelType types are :" << endl;
    Info<< dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<granularPressureModel>(cstrIter()(dict));
}
