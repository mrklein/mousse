// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "realizable_ke.hpp"
#include "bound.hpp"


namespace mousse {
namespace RASModels {

// Private Member Functions 
template<class BasicTurbulenceModel>
tmp<volScalarField> realizableKE<BasicTurbulenceModel>::rCmu
(
  const volTensorField& gradU,
  const volScalarField& S2,
  const volScalarField& magS
)
{
  tmp<volSymmTensorField> tS = dev(symm(gradU));
  const volSymmTensorField& S = tS();
  const dimensionedScalar Wsmall{"small", {0, 0, -3, 0, 0}, SMALL};
  volScalarField W{(2*sqrt(2.0))*((S&S)&&S)/(magS*S2 + Wsmall)};
  tS.clear();
  volScalarField phis
  {
    (1.0/3.0)*acos(min(max(sqrt(6.0)*W, -scalar(1)), scalar(1)))
  };
  volScalarField As{sqrt(6.0)*cos(phis)};
  volScalarField Us{sqrt(S2/2.0 + magSqr(skew(gradU)))};
  return 1.0/(A0_ + As*Us*k_/epsilon_);
}


template<class BasicTurbulenceModel>
void realizableKE<BasicTurbulenceModel>::correctNut
(
  const volTensorField& gradU,
  const volScalarField& S2,
  const volScalarField& magS
)
{
  this->nut_ = rCmu(gradU, S2, magS)*sqr(k_)/epsilon_;
  this->nut_.correctBoundaryConditions();
  BasicTurbulenceModel::correctNut();
}


template<class BasicTurbulenceModel>
void realizableKE<BasicTurbulenceModel>::correctNut()
{
  tmp<volTensorField> tgradU = fvc::grad(this->U_);
  volScalarField S2{2*magSqr(dev(symm(tgradU())))};
  volScalarField magS{sqrt(S2)};
  correctNut(tgradU(), S2, magS);
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix> realizableKE<BasicTurbulenceModel>::kSource() const
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
tmp<fvScalarMatrix> realizableKE<BasicTurbulenceModel>::epsilonSource() const
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
realizableKE<BasicTurbulenceModel>::realizableKE
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
  A0_
  {
    dimensioned<scalar>::lookupOrAddToDict("A0", this->coeffDict_, 4.0)
  },
  C2_
  {
    dimensioned<scalar>::lookupOrAddToDict("C2", this->coeffDict_, 1.9)
  },
  sigmak_
  {
    dimensioned<scalar>::lookupOrAddToDict("sigmak", this->coeffDict_, 1.0)
  },
  sigmaEps_
  {
    dimensioned<scalar>::lookupOrAddToDict("sigmaEps", this->coeffDict_, 1.2)
  },
  k_
  {
    {
      "k",
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
      "epsilon",
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
    correctNut();
    this->printCoeffs(type);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool realizableKE<BasicTurbulenceModel>::read()
{
  if (eddyViscosity<RASModel<BasicTurbulenceModel>>::read()) {
    Cmu_.readIfPresent(this->coeffDict());
    A0_.readIfPresent(this->coeffDict());
    C2_.readIfPresent(this->coeffDict());
    sigmak_.readIfPresent(this->coeffDict());
    sigmaEps_.readIfPresent(this->coeffDict());
    return true;
  }
  return false;
}


template<class BasicTurbulenceModel>
void realizableKE<BasicTurbulenceModel>::correct()
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
  volScalarField S2{2*magSqr(dev(symm(tgradU())))};
  volScalarField magS{sqrt(S2)};
  volScalarField eta{magS*k_/epsilon_};
  volScalarField C1{max(eta/(scalar(5) + eta), scalar(0.43))};
  volScalarField G{this->GName(), nut*(tgradU() && dev(twoSymm(tgradU())))};
  // Update epsilon and G at the wall
  epsilon_.boundaryField().updateCoeffs();
  // Dissipation equation
  tmp<fvScalarMatrix> epsEqn {
    fvm::ddt(alpha, rho, epsilon_)
  + fvm::div(alphaRhoPhi, epsilon_)
  - fvm::laplacian(alpha*rho*DepsilonEff(), epsilon_)
  ==
    C1*alpha*rho*magS*epsilon_
  - fvm::Sp
    (
      C2_*alpha*rho*epsilon_/(k_ + sqrt(this->nu()*epsilon_)),
      epsilon_
    )
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
  - fvm::SuSp(2.0/3.0*alpha*rho*divU, k_)
  - fvm::Sp(alpha*rho*epsilon_/k_, k_)
  + kSource()
  };
  kEqn().relax();
  solve(kEqn);
  bound(k_, this->kMin_);
  correctNut(tgradU(), S2, magS);
}

}  // namespace RASModels
}  // namespace mousse

