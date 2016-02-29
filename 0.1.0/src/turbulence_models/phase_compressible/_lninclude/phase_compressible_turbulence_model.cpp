// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_compressible_turbulence_model.hpp"
// Constructors 
template<class TransportModel>
mousse::PhaseCompressibleTurbulenceModel<TransportModel>::
PhaseCompressibleTurbulenceModel
(
  const word& /*type*/,
  const volScalarField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  TurbulenceModel
  <
    volScalarField,
    volScalarField,
    compressibleTurbulenceModel,
    transportModel
  >
  (
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  )
{}
// Selectors
template<class TransportModel>
mousse::autoPtr<mousse::PhaseCompressibleTurbulenceModel<TransportModel> >
mousse::PhaseCompressibleTurbulenceModel<TransportModel>::New
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  return autoPtr<PhaseCompressibleTurbulenceModel>
  (
    static_cast<PhaseCompressibleTurbulenceModel*>(
    TurbulenceModel
    <
      volScalarField,
      volScalarField,
      compressibleTurbulenceModel,
      transportModel
    >::New
    (
      alpha,
      rho,
      U,
      alphaRhoPhi,
      phi,
      transport,
      propertiesName
    ).ptr())
  );
}
// Member Functions 
template<class TransportModel>
mousse::tmp<mousse::volScalarField>
mousse::PhaseCompressibleTurbulenceModel<TransportModel>::pPrime() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("pPrime", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      this->mesh_,
      dimensionedScalar("pPrimef", dimPressure, 0.0)
    )
  );
}
template<class TransportModel>
mousse::tmp<mousse::surfaceScalarField>
mousse::PhaseCompressibleTurbulenceModel<TransportModel>::pPrimef() const
{
  return tmp<surfaceScalarField>
  (
    new surfaceScalarField
    (
      IOobject
      (
        IOobject::groupName("pPrimef", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      this->mesh_,
      dimensionedScalar("pPrimef", dimPressure, 0.0)
    )
  );
}
