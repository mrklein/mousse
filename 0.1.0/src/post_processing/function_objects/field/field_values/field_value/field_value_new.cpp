// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value.hpp"
// Member Functions 
mousse::autoPtr<mousse::fieldValue> mousse::fieldValue::New
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles,
  const bool output
)
{
  const word modelType(dict.lookup("type"));
  if (output)
  {
    Info<< "Selecting " << typeName << " " << modelType << endl;
  }
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "fieldValue::New"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "Unknown " << typeName << " type "
      << modelType << nl << nl
      << "Valid " << typeName << " types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<fieldValue>
  (
    cstrIter()
    (
      name,
      obr,
      dict,
      loadFromFiles
    )
  );
}
