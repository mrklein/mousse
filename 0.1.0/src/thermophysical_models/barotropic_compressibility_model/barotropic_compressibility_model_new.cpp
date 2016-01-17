// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "barotropic_compressibility_model.hpp"
mousse::autoPtr<mousse::barotropicCompressibilityModel>
mousse::barotropicCompressibilityModel::New
(
  const dictionary& compressibilityProperties,
  const volScalarField& gamma,
  const word& psiName
)
{
  const word modelType
  (
    compressibilityProperties.lookup("barotropicCompressibilityModel")
  );
  Info<< "Selecting compressibility model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "barotropicCompressibilityModel::New(const volScalarField&)"
    )   << "Unknown barotropicCompressibilityModel type "
      << modelType << nl << nl
      << "Valid barotropicCompressibilityModels are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<barotropicCompressibilityModel>
  (
    cstrIter()(compressibilityProperties, gamma, psiName)
  );
}
