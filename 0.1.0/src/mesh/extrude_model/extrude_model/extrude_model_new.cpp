// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extrude_model.hpp"
mousse::autoPtr<mousse::extrudeModel> mousse::extrudeModel::New
(
  const dictionary& dict
)
{
  const word modelType(dict.lookup("extrudeModel"));
  Info<< "Selecting extrudeModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "extrudeModel::New(const dictionary&)"
    )
    << "Unknown extrudeModel type "
    << modelType << nl << nl
    << "Valid extrudeModel types are :" << nl
    << dictionaryConstructorTablePtr_->sortedToc() << nl
    << exit(FatalError);
  }
  return autoPtr<extrudeModel>(cstrIter()(dict));
}
