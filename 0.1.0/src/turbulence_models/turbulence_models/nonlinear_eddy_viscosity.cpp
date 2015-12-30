// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nonlinear_eddy_viscosity.hpp"
#include "fvc.hpp"
#include "fvm.hpp"
// Constructors 
template<class BasicTurbulenceModel>
mousse::nonlinearEddyViscosity<BasicTurbulenceModel>::nonlinearEddyViscosity
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
  eddyViscosity<BasicTurbulenceModel>
  (
    modelName,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  ),
  nonlinearStress_
  (
    IOobject
    (
      IOobject::groupName("nonlinearStress", U.group()),
      this->runTime_.timeName(),
      this->mesh_
    ),
    this->mesh_,
    dimensionedSymmTensor
    (
      "nonlinearStress",
      sqr(dimVelocity),
      symmTensor::zero
    )
  )
{}
// Member Functions 
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::nonlinearEddyViscosity<BasicTurbulenceModel>::R() const
{
  tmp<volSymmTensorField> tR
  (
    eddyViscosity<BasicTurbulenceModel>::R()
  );
  tR() += nonlinearStress_;
  return tR;
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::nonlinearEddyViscosity<BasicTurbulenceModel>::devRhoReff() const
{
  tmp<volSymmTensorField> tdevRhoReff
  (
    eddyViscosity<BasicTurbulenceModel>::devRhoReff()
  );
  tdevRhoReff() += this->rho_*nonlinearStress_;
  return tdevRhoReff;
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::nonlinearEddyViscosity<BasicTurbulenceModel>::divDevRhoReff
(
  volVectorField& U
) const
{
  return
  (
    fvc::div(this->rho_*nonlinearStress_)
   + eddyViscosity<BasicTurbulenceModel>::divDevRhoReff(U)
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::nonlinearEddyViscosity<BasicTurbulenceModel>::divDevRhoReff
(
  const volScalarField& rho,
  volVectorField& U
) const
{
  return
  (
    fvc::div(rho*nonlinearStress_)
   + eddyViscosity<BasicTurbulenceModel>::divDevRhoReff(rho, U)
  );
}
