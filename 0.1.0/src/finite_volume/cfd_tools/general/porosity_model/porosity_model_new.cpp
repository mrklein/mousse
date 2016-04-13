// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "porosity_model.hpp"


// Constructors 
mousse::autoPtr<mousse::porosityModel> mousse::porosityModel::New
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& cellZoneName
)
{
  const word modelType{dict.lookup("type")};
  Info << "Porosity region " << name << ":" << nl
    << "    selecting model: " << modelType << endl;
  meshConstructorTable::iterator cstrIter =
    meshConstructorTablePtr_->find(modelType);
  if (cstrIter == meshConstructorTablePtr_->end()) {
    FATAL_ERROR_IN_FUNCTION
      << "Unknown " << typeName << " type " << modelType << nl << nl
      << "Valid " << typeName << " types are:" << nl
      << meshConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<porosityModel>
  {
    cstrIter()
    (
      name,
      modelType,
      mesh,
      dict,
      cellZoneName
    )
  };
}

