// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_incompressible_turbulence_model.hpp"
// Constructors 
template<class TransportModel>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::
PhaseIncompressibleTurbulenceModel
(
  const word& type,
  const volScalarField& alpha,
  const geometricOneField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const TransportModel& transportModel,
  const word& propertiesName
)
:
  TurbulenceModel
  <
    volScalarField,
    geometricOneField,
    incompressibleTurbulenceModel,
    TransportModel
  >
  (
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transportModel,
    propertiesName
  )
{}
// Selectors
template<class TransportModel>
mousse::autoPtr<mousse::PhaseIncompressibleTurbulenceModel<TransportModel> >
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::New
(
  const volScalarField& alpha,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const TransportModel& transportModel,
  const word& propertiesName
)
{
  return autoPtr<PhaseIncompressibleTurbulenceModel>
  (
    static_cast<PhaseIncompressibleTurbulenceModel*>(
    TurbulenceModel
    <
      volScalarField,
      geometricOneField,
      incompressibleTurbulenceModel,
      TransportModel
    >::New
    (
      alpha,
      geometricOneField(),
      U,
      alphaRhoPhi,
      phi,
      transportModel,
      propertiesName
    ).ptr())
  );
}
// Member Functions 
template<class TransportModel>
mousse::tmp<mousse::volScalarField>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::pPrime() const
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
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::pPrimef() const
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
template<class TransportModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::devReff() const
{
  return devRhoReff();
}
template<class TransportModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::divDevReff
(
  volVectorField& U
) const
{
  return divDevRhoReff(U);
}
template<class TransportModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::
devRhoReff() const
{
  NOT_IMPLEMENTED
  (
    "PhaseIncompressibleTurbulenceModel<TransportModel>::"
    "devRhoReff()"
  );
  return devReff();
}
template<class TransportModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::PhaseIncompressibleTurbulenceModel<TransportModel>::
divDevRhoReff
(
  volVectorField& U
) const
{
  NOT_IMPLEMENTED
  (
    "PhaseIncompressibleTurbulenceModel<TransportModel>::"
    "divDevRhoReff(volVectorField& U)"
  );
  return divDevReff(U);
}
