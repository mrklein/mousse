// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "scatter_model.hpp"


// Constructors 
mousse::autoPtr<mousse::radiation::scatterModel> mousse::radiation::scatterModel::New
(
  const dictionary& dict,
  const fvMesh& mesh
)
{
  const word modelType{dict.lookup("scatterModel")};
  Info << "Selecting scatterModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "scatterModel::New(const dictionary&, const fvMesh&)"
    )
    << "Unknown scatterModel type "
    << modelType << nl << nl
    << "Valid scatterModel types are :" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }

  return autoPtr<scatterModel>{cstrIter()(dict, mesh)};
}

