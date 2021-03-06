// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "deardorff_diff_stress.hpp"


namespace mousse {
namespace LESModels{

// Protected Member Functions 
template<class BasicTurbulenceModel>
void DeardorffDiffStress<BasicTurbulenceModel>::correctNut()
{
  this->nut_ = Ck_*sqrt(this->k())*this->delta();
  this->nut_.correctBoundaryConditions();
  BasicTurbulenceModel::correctNut();
}


// Constructors 
template<class BasicTurbulenceModel>
DeardorffDiffStress<BasicTurbulenceModel>::DeardorffDiffStress
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
  ReynoldsStress<LESModel<BasicTurbulenceModel> >
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
  Cm_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Cm",
      this->coeffDict_,
      4.13
    )
  },
  Ce_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Ce",
      this->coeffDict_,
      1.05
    )
  },
  Cs_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Cs",
      this->coeffDict_,
      0.25
    )
  }
{
  if (type == typeName) {
    this->boundNormalStress(this->R_);
    correctNut();
    this->printCoeffs(type);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool DeardorffDiffStress<BasicTurbulenceModel>::read()
{
  if (ReynoldsStress<LESModel<BasicTurbulenceModel> >::read()) {
    Ck_.readIfPresent(this->coeffDict());
    Cm_.readIfPresent(this->coeffDict());
    Ce_.readIfPresent(this->coeffDict());
    Cs_.readIfPresent(this->coeffDict());
    return true;
  }
  return false;
}


template<class BasicTurbulenceModel>
tmp<volScalarField> DeardorffDiffStress<BasicTurbulenceModel>::epsilon() const
{
  volScalarField k{this->k()};
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
void DeardorffDiffStress<BasicTurbulenceModel>::correct()
{
  if (!this->turbulence_) {
    return;
  }
  // Local references
  const alphaField& alpha = this->alpha_;
  const rhoField& rho = this->rho_;
  const surfaceScalarField& alphaRhoPhi = this->alphaRhoPhi_;
  const volVectorField& U = this->U_;
  volSymmTensorField& R = this->R_;
  ReynoldsStress<LESModel<BasicTurbulenceModel> >::correct();
  tmp<volTensorField> tgradU{fvc::grad(U)};
  const volTensorField& gradU = tgradU();
  volSymmTensorField D{symm(gradU)};
  volSymmTensorField P{-twoSymm(R & gradU)};
  volScalarField k{this->k()};
  tmp<fvSymmTensorMatrix> REqn {
    fvm::ddt(alpha, rho, R)
  + fvm::div(alphaRhoPhi, R)
  - fvm::laplacian(I*this->nu() + Cs_*(k/this->epsilon())*R, R)
  + fvm::Sp(Cm_*alpha*rho*sqrt(k)/this->delta(), R)
  ==
    alpha*rho*P
  + (4.0/5.0)*alpha*rho*k*D
  - ((2.0/3.0)*(1.0 - Cm_/this->Ce_)*I)*(alpha*rho*this->epsilon())
  };
  REqn().relax();
  REqn().solve();
  this->boundNormalStress(R);
  correctNut();
}

}  // namespace LESModels
}  // namespace mousse

