// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pdr_drag_model.hpp"
mousse::autoPtr<mousse::PDRDragModel> mousse::PDRDragModel::New
(
  const dictionary& PDRProperties,
  const compressible::RASModel& turbulence,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi
)
{
  const word modelType{PDRProperties.lookup("PDRDragModel")};
  Info<< "Selecting flame-wrinkling model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "PDRDragModel::New"
    )
    << "Unknown PDRDragModel type "
    << modelType << nl << nl
    << "Valid  PDRDragModels are : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return {cstrIter()(PDRProperties, turbulence, rho, U, phi)};
}
