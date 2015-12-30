// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "viscosity_model.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
mousse::autoPtr<mousse::viscosityModel> mousse::viscosityModel::New
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
{
  const word modelType(viscosityProperties.lookup("transportModel"));
  Info<< "Selecting incompressible transport model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "viscosityModel::New(const volVectorField&, "
      "const surfaceScalarField&)"
    )   << "Unknown viscosityModel type "
      << modelType << nl << nl
      << "Valid viscosityModels are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<viscosityModel>
    (cstrIter()(name, viscosityProperties, U, phi));
}
