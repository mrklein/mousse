// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_turbulence_model.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


// Constructors 
template
<
  class Alpha,
  class Rho,
  class BasicTurbulenceModel,
  class TransportModel
>
mousse::TurbulenceModel<Alpha, Rho, BasicTurbulenceModel, TransportModel>::
TurbulenceModel
(
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  BasicTurbulenceModel
  {
    rho,
    U,
    alphaRhoPhi,
    phi,
    propertiesName
  },
  alpha_{alpha},
  transport_{transport}
{}


// Selectors
template
<
  class Alpha,
  class Rho,
  class BasicTurbulenceModel,
  class TransportModel
>
mousse::autoPtr
<
  mousse::TurbulenceModel<Alpha, Rho, BasicTurbulenceModel, TransportModel>
>
mousse::TurbulenceModel<Alpha, Rho, BasicTurbulenceModel, TransportModel>::New
(
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  // get model name, but do not register the dictionary
  // otherwise it is registered in the database twice
  const word modelType
  {
    IOdictionary
    {
      // IOobject
      {
        IOobject::groupName(propertiesName, U.group()),
        U.time().constant(),
        U.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    }.lookup("simulationType")
  };
  Info << "Selecting turbulence model type " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "TurbulenceModel::New"
      "(const alphaField&, const rhoField&, "
      "const volVectorField&, const surfaceScalarField&, "
      "transportModel&, const word&)"
    )
    << "Unknown TurbulenceModel type "
    << modelType << nl << nl
    << "Valid TurbulenceModel types:" << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return
    autoPtr<TurbulenceModel>
    {
      cstrIter()(alpha, rho, U, alphaRhoPhi, phi, transport, propertiesName)
    };
}

