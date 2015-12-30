// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model_function_object.hpp"
mousse::autoPtr<mousse::regionModels::regionModelFunctionObject>
mousse::regionModels::regionModelFunctionObject::New
(
  const dictionary& dict,
  regionModel& owner,
  const word& modelName
)
{
  const word modelType = dict.subDict(modelName).lookup("type");
  Info<< "        " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "regionModelFunctionObject::New"
      "("
        "const dictionary&, "
        "regionModel&, "
        "const word&"
      ")"
    )   << "Unknown region model function type "
      << modelType << nl << nl
      << "Valid region model function types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return
    autoPtr<regionModelFunctionObject>
    (
      cstrIter()
      (
        dict.subDict(modelName),
        owner
      )
    );
}
