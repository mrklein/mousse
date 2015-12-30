// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "soot_model.hpp"
// Constructors 
mousse::autoPtr<mousse::radiation::sootModel>
mousse::radiation::sootModel::New
(
  const dictionary& dict,
  const fvMesh& mesh
)
{
  word modelType("none");
  if (dict.found("sootModel"))
  {
    dict.lookup("sootModel") >> modelType;
    Info<< "Selecting sootModel " << modelType << endl;
  }
  dictionaryConstructorTable::iterator cstrIter =
      dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "sootModel::New(const dictionary&, const fvMesh&)"
    )   << "Unknown sootModel type "
      << modelType << nl << nl
      << "Valid sootModel types are :" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  const label tempOpen = modelType.find('<');
  const word className = modelType(0, tempOpen);
  return autoPtr<sootModel>(cstrIter()(dict, mesh, className));
}
