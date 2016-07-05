// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_viscous_stress.hpp"
#include "fvc.hpp"
#include "fvm.hpp"


// Constructors 
template<class BasicTurbulenceModel>
mousse::linearViscousStress<BasicTurbulenceModel>::linearViscousStress
(
  const word& modelName,
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
    modelName,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  }
{}


// Member Functions 
template<class BasicTurbulenceModel>
bool mousse::linearViscousStress<BasicTurbulenceModel>::read()
{
  return BasicTurbulenceModel::read();
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::linearViscousStress<BasicTurbulenceModel>::devRhoReff() const
{
  return
    tmp<volSymmTensorField>
    {
      new volSymmTensorField
      {
        {
          IOobject::groupName("devRhoReff", this->U_.group()),
          this->runTime_.timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        (-(this->alpha_*this->rho_*this->nuEff()))
        *dev(twoSymm(fvc::grad(this->U_)))
      }
    };
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::linearViscousStress<BasicTurbulenceModel>::divDevRhoReff
(
  volVectorField& U
) const
{
  return
    (
      -fvm::laplacian(this->alpha_*this->rho_*this->nuEff(), U)
      - fvc::div((this->alpha_*this->rho_*this->nuEff())*dev2(T(fvc::grad(U))))
    );
}


template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::linearViscousStress<BasicTurbulenceModel>::divDevRhoReff
(
  const volScalarField& rho,
  volVectorField& U
) const
{
  return
    (
      -fvm::laplacian(this->alpha_*rho*this->nuEff(), U)
      - fvc::div((this->alpha_*rho*this->nuEff())*dev2(T(fvc::grad(U))))
    );
}


template<class BasicTurbulenceModel>
void mousse::linearViscousStress<BasicTurbulenceModel>::correct()
{
  BasicTurbulenceModel::correct();
}

