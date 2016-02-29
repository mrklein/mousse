// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixture_viscosity_model.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"

mousse::autoPtr<mousse::mixtureViscosityModel> mousse::mixtureViscosityModel::New
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
{
  const word modelType{viscosityProperties.lookup("transportModel")};
  Info<< "Selecting incompressible transport model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "mixtureViscosityModel::New(const volVectorField&, "
      "const surfaceScalarField&)"
    )
    << "Unknown mixtureViscosityModel type "
    << modelType << nl << nl
    << "Valid mixtureViscosityModels are : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return {cstrIter()(name, viscosityProperties, U, phi)};
}
