// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wale.hpp"


namespace mousse {
namespace LESModels {

// Protected Member Functions 
template<class BasicTurbulenceModel>
tmp<volSymmTensorField> WALE<BasicTurbulenceModel>::Sd
(
  const volTensorField& gradU
) const
{
  return dev(symm(gradU & gradU));
}


template<class BasicTurbulenceModel>
tmp<volScalarField> WALE<BasicTurbulenceModel>::k
(
  const volTensorField& gradU
) const
{
  volScalarField magSqrSd{magSqr(Sd(gradU))};
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          IOobject::groupName("k", this->U_.group()),
          this->runTime_.timeName(),
          this->mesh_
        },
        sqr(sqr(Cw_)*this->delta()/Ck_)*
        (
          pow3(magSqrSd)
          /(
            sqr
            (
              pow(magSqr(symm(gradU)), 5.0/2.0)
              + pow(magSqrSd, 5.0/4.0)
            )
            + dimensionedScalar
            (
              "SMALL",
              dimensionSet(0, 0, -10, 0, 0),
              SMALL
            )
           )
        )
      }
    };
}


template<class BasicTurbulenceModel>
void WALE<BasicTurbulenceModel>::correctNut()
{
  this->nut_ = Ck_*this->delta()*sqrt(this->k(fvc::grad(this->U_)));
  this->nut_.correctBoundaryConditions();
  BasicTurbulenceModel::correctNut();
}


// Constructors 
template<class BasicTurbulenceModel>
WALE<BasicTurbulenceModel>::WALE
(
  const alphaField& alpha,
  const rhoField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& transport,
  const word& propertiesName,
  const word& type
)
:
  LESeddyViscosity<BasicTurbulenceModel>
  {
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName
  },
  Ck_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Ck",
      this->coeffDict_,
      0.094
    )
  },
  Cw_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Cw",
      this->coeffDict_,
      0.325
    )
  }
{
  if (type == typeName) {
    correctNut();
    this->printCoeffs(type);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool WALE<BasicTurbulenceModel>::read()
{
  if (LESeddyViscosity<BasicTurbulenceModel>::read()) {
    Ck_.readIfPresent(this->coeffDict());
    Cw_.readIfPresent(this->coeffDict());
    return true;
  }
  return false;
}


template<class BasicTurbulenceModel>
tmp<volScalarField> WALE<BasicTurbulenceModel>::epsilon() const
{
  volScalarField k{this->k(fvc::grad(this->U_))};
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          IOobject::groupName("epsilon", this->U_.group()),
          this->runTime_.timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        this->Ce_*k*sqrt(k)/this->delta()
      }
    };
}


template<class BasicTurbulenceModel>
void WALE<BasicTurbulenceModel>::correct()
{
  LESeddyViscosity<BasicTurbulenceModel>::correct();
  correctNut();
}


}  // namespace LESModels
}  // namespace mousse

