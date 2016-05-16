// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_diffusivity.hpp"


// Constructors 
template<class BasicTurbulenceModel>
mousse::ThermalDiffusivity<BasicTurbulenceModel>::ThermalDiffusivity
(
  const word& type,
  const alphaField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
:
  BasicTurbulenceModel
  {
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  }
{}


// Selectors
template<class BasicTurbulenceModel>
mousse::autoPtr<mousse::ThermalDiffusivity<BasicTurbulenceModel> >
mousse::ThermalDiffusivity<BasicTurbulenceModel>::New
(
  const alphaField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  return
    autoPtr<ThermalDiffusivity>
    {
      static_cast<ThermalDiffusivity*>
      (
        BasicTurbulenceModel::New
        (
          alpha,
          rho,
          U,
          alphaRhoPhi,
          phi,
          transport,
          propertiesName
        ).ptr()
      )
    };
}


template<class BasicTurbulenceModel>
mousse::autoPtr<mousse::ThermalDiffusivity<BasicTurbulenceModel> >
mousse::ThermalDiffusivity<BasicTurbulenceModel>::New
(
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName
)
{
  return
    autoPtr<ThermalDiffusivity>
    {
      static_cast<ThermalDiffusivity*>
      (
        BasicTurbulenceModel::New
        (
          rho,
          U,
          phi,
          transport,
          propertiesName
        ).ptr()
      )
    };
}


// Member Functions 
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::ThermalDiffusivity<BasicTurbulenceModel>::alphat() const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          IOobject::groupName("alphat", this->U_.group()),
          this->runTime_.timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        this->mesh_,
        {"alphat", dimDensity*dimViscosity, 0.0}
      }
    };
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::scalarField>
mousse::ThermalDiffusivity<BasicTurbulenceModel>::alphat
(
  const label patchi
) const
{
  return
    tmp<scalarField>
    {
      new scalarField{this->mesh_.boundary()[patchi].size(), 0.0}
    };
}

