// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "trim_model.hpp"
// Constructors 
mousse::autoPtr<mousse::trimModel> mousse::trimModel::New
(
  const fv::rotorDiskSource& rotor,
  const dictionary& dict
)
{
  const word modelType(dict.lookup(typeName));
  Info<< "    Selecting " << typeName << " " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "trimModel::New(const rotorDiskSource&, const dictionary&)"
    )   << "Unknown " << typeName << " type "
      << modelType << nl << nl
      << "Valid " << typeName << " types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<trimModel>(cstrIter()(rotor, dict));
}
