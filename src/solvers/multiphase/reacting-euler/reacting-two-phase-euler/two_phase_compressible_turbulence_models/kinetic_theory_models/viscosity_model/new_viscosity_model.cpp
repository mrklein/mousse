// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "viscosity_model.hpp"
// Selectors
mousse::autoPtr<mousse::kineticTheoryModels::viscosityModel>
mousse::kineticTheoryModels::viscosityModel::New
(
  const dictionary& dict
)
{
  word viscosityModelType(dict.lookup("viscosityModel"));
  Info<< "Selecting viscosityModel "
    << viscosityModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(viscosityModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalError
      << "viscosityModel::New(const dictionary&) : " << endl
      << "    unknown viscosityModelType type "
      << viscosityModelType
      << ", constructor not in hash table" << endl << endl
      << "    Valid viscosityModelType types are :" << endl;
    Info<< dictionaryConstructorTablePtr_->sortedToc() << abort(FatalError);
  }
  return autoPtr<viscosityModel>(cstrIter()(dict));
}
