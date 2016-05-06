// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "k_epsilon.hpp"
#include "bound.hpp"


namespace mousse {
namespace RASModels {

// Protected Member Functions 
template<class BasicTurbulenceModel>
void kEpsilon<BasicTurbulenceModel>::correctNut()
{
  this->nut_ = Cmu_*sqr(k_)/epsilon_;
  this->nut_.correctBoundaryConditions();
  BasicTurbulenceModel::correctNut();
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix> kEpsilon<BasicTurbulenceModel>::kSource() const
{
  return
    tmp<fvScalarMatrix>
    {
      new fvScalarMatrix
      {
        k_,
        dimVolume*this->rho_.dimensions()*k_.dimensions()/dimTime
      }
    };
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix> kEpsilon<BasicTurbulenceModel>::epsilonSource() const
{
  return
    tmp<fvScalarMatrix>
    {
      new fvScalarMatrix
      {
        epsilon_,
        dimVolume*this->rho_.dimensions()*epsilon_.dimensions()/dimTime
      }
    };
}


// Constructors 
template<class BasicTurbulenceModel>
kEpsilon<BasicTurbulenceModel>::kEpsilon
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
  eddyViscosity<RASModel<BasicTurbulenceModel>>
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
  Cmu_
  {
    dimensioned<scalar>::lookupOrAddToDict("Cmu", this->coeffDict_, 0.09)
  },
  C1_
  {
    dimensioned<scalar>::lookupOrAddToDict("C1", this->coeffDict_, 1.44)
  },
  C2_
  {
    dimensioned<scalar>::lookupOrAddToDict("C2", this->coeffDict_, 1.92)
  },
  C3_
  {
    dimensioned<scalar>::lookupOrAddToDict("C3", this->coeffDict_, -0.33)
  },
  sigmak_
  {
    dimensioned<scalar>::lookupOrAddToDict("sigmak", this->coeffDict_, 1.0)
  },
  sigmaEps_
  {
    dimensioned<scalar>::lookupOrAddToDict("sigmaEps", this->coeffDict_, 1.3)
  },
  k_
  {
    {
      IOobject::groupName("k", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  },
  epsilon_
  {
    {
      IOobject::groupName("epsilon", U.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  }
{
  bound(k_, this->kMin_);
  bound(epsilon_, this->epsilonMin_);
  if (type == typeName) {
    this->printCoeffs(type);
    correctNut();
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool kEpsilon<BasicTurbulenceModel>::read()
{
  if (eddyViscosity<RASModel<BasicTurbulenceModel>>::read()) {
    Cmu_.readIfPresent(this->coeffDict());
    C1_.readIfPresent(this->coeffDict());
    C2_.readIfPresent(this->coeffDict());
    C3_.readIfPresent(this->coeffDict());
    sigmak_.readIfPresent(this->coeffDict());
    sigmaEps_.readIfPresent(this->coeffDict());
    return true;
  }
  return false;
}


template<class BasicTurbulenceModel>
void kEpsilon<BasicTurbulenceModel>::correct()
{
  if (!this->turbulence_) {
    return;
  }
  // Local references
  const alphaField& alpha = this->alpha_;
  const rhoField& rho = this->rho_;
  const surfaceScalarField& alphaRhoPhi = this->alphaRhoPhi_;
  const volVectorField& U = this->U_;
  volScalarField& nut = this->nut_;
  eddyViscosity<RASModel<BasicTurbulenceModel>>::correct();
  volScalarField divU{fvc::div(fvc::absolute(this->phi(), U))};
  tmp<volTensorField> tgradU = fvc::grad(U);
  volScalarField G{this->GName(), nut*(dev(twoSymm(tgradU())) && tgradU())};
  tgradU.clear();
  // Update epsilon and G at the wall
  epsilon_.boundaryField().updateCoeffs();
  // Dissipation equation
  tmp<fvScalarMatrix> epsEqn {
    fvm::ddt(alpha, rho, epsilon_)
  + fvm::div(alphaRhoPhi, epsilon_)
  - fvm::laplacian(alpha*rho*DepsilonEff(), epsilon_)
  ==
    C1_*alpha*rho*G*epsilon_/k_
  - fvm::SuSp(((2.0/3.0)*C1_ + C3_)*alpha*rho*divU, epsilon_)
  - fvm::Sp(C2_*alpha*rho*epsilon_/k_, epsilon_)
  + epsilonSource()
  };
  epsEqn().relax();
  epsEqn().boundaryManipulate(epsilon_.boundaryField());
  solve(epsEqn);
  bound(epsilon_, this->epsilonMin_);
  // Turbulent kinetic energy equation
  tmp<fvScalarMatrix> kEqn {
    fvm::ddt(alpha, rho, k_)
  + fvm::div(alphaRhoPhi, k_)
  - fvm::laplacian(alpha*rho*DkEff(), k_)
  ==
    alpha*rho*G
  - fvm::SuSp((2.0/3.0)*alpha*rho*divU, k_)
  - fvm::Sp(alpha*rho*epsilon_/k_, k_)
  + kSource()
  };
  kEqn().relax();
  solve(kEqn);
  bound(k_, this->kMin_);
  correctNut();
}

}  // namespace RASModels
}  // namespace mousse
