// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laminar.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fvc_grad.hpp"
#include "fvc_div.hpp"
#include "fvm_laplacian.hpp"
// Constructors 
template<class BasicTurbulenceModel>
mousse::laminar<BasicTurbulenceModel>::laminar
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
  linearViscousStress<BasicTurbulenceModel>
  (
    typeName,
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
template<class BasicTurbulenceModel>
mousse::autoPtr<mousse::laminar<BasicTurbulenceModel> >
mousse::laminar<BasicTurbulenceModel>::New
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
  return autoPtr<laminar>
  (
    new laminar
    (
      alpha,
      rho,
      U,
      alphaRhoPhi,
      phi,
      transport,
      propertiesName
    )
  );
}
// Member Functions 
template<class BasicTurbulenceModel>
const mousse::dictionary&
mousse::laminar<BasicTurbulenceModel>::coeffDict() const
{
  return dictionary::null;
}
template<class BasicTurbulenceModel>
bool mousse::laminar<BasicTurbulenceModel>::read()
{
  return true;
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::laminar<BasicTurbulenceModel>::nut() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("nut", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedScalar("nut", dimViscosity, 0.0)
    )
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::scalarField>
mousse::laminar<BasicTurbulenceModel>::nut
(
  const label patchi
) const
{
  return tmp<scalarField>
  (
    new scalarField(this->mesh_.boundary()[patchi].size(), 0.0)
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::laminar<BasicTurbulenceModel>::nuEff() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject::groupName("nuEff", this->U_.group()), this->nu()
    )
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::scalarField>
mousse::laminar<BasicTurbulenceModel>::nuEff
(
  const label patchi
) const
{
  return this->nu(patchi);
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::laminar<BasicTurbulenceModel>::k() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("k", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedScalar("k", sqr(this->U_.dimensions()), 0.0)
    )
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volScalarField>
mousse::laminar<BasicTurbulenceModel>::epsilon() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("epsilon", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedScalar
      (
        "epsilon", sqr(this->U_.dimensions())/dimTime, 0.0
      )
    )
  );
}
template<class BasicTurbulenceModel>
mousse::tmp<mousse::volSymmTensorField>
mousse::laminar<BasicTurbulenceModel>::R() const
{
  return tmp<volSymmTensorField>
  (
    new volSymmTensorField
    (
      IOobject
      (
        IOobject::groupName("R", this->U_.group()),
        this->runTime_.timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedSymmTensor
      (
        "R", sqr(this->U_.dimensions()), symmTensor::zero
      )
    )
  );
}
template<class BasicTurbulenceModel>
void mousse::laminar<BasicTurbulenceModel>::correct()
{
  BasicTurbulenceModel::correct();
}
