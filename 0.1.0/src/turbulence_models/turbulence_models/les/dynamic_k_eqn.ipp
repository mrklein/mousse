// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_k_eqn.hpp"


namespace mousse {
namespace LESModels {

// Private Member Functions 
template<class BasicTurbulenceModel>
volScalarField dynamicKEqn<BasicTurbulenceModel>::Ck
(
  const volSymmTensorField& D,
  const volScalarField& KK
) const
{
  const volSymmTensorField LL
  {
    simpleFilter_(dev(filter_(sqr(this->U_)) - (sqr(filter_(this->U_)))))
  };
  const volSymmTensorField MM
  {
    simpleFilter_(-2.0*this->delta()*sqrt(KK)*filter_(D))
  };
  const volScalarField Ck
  {
    simpleFilter_(0.5*(LL && MM))
    /(simpleFilter_(magSqr(MM))
      + dimensionedScalar("small", sqr(MM.dimensions()), VSMALL))
  };
  tmp<volScalarField> tfld = 0.5*(mag(Ck) + Ck);
  return tfld();
}


template<class BasicTurbulenceModel>
volScalarField dynamicKEqn<BasicTurbulenceModel>::Ce
(
  const volSymmTensorField& D,
  const volScalarField& KK
) const
{
  const volScalarField Ce
  {
    simpleFilter_(this->nuEff()*(filter_(magSqr(D)) - magSqr(filter_(D))))
    /simpleFilter_(pow(KK, 1.5)/(2.0*this->delta()))
  };
  tmp<volScalarField> tfld = 0.5*(mag(Ce) + Ce);
  return tfld();
}


template<class BasicTurbulenceModel>
volScalarField dynamicKEqn<BasicTurbulenceModel>::Ce() const
{
  const volSymmTensorField D{dev(symm(fvc::grad(this->U_)))};
  volScalarField KK
  {
    0.5*(filter_(magSqr(this->U_)) - magSqr(filter_(this->U_)))
  };
  KK.max(dimensionedScalar{"small", KK.dimensions(), SMALL});
  return Ce(D, KK);
}


template<class BasicTurbulenceModel>
void dynamicKEqn<BasicTurbulenceModel>::correctNut
(
  const volSymmTensorField& D,
  const volScalarField& KK
)
{
  this->nut_ = Ck(D, KK)*sqrt(k_)*this->delta();
  this->nut_.correctBoundaryConditions();
}


template<class BasicTurbulenceModel>
void dynamicKEqn<BasicTurbulenceModel>::correctNut()
{
  const volScalarField KK
  {
    0.5*(filter_(magSqr(this->U_)) - magSqr(filter_(this->U_)))
  };
  correctNut(symm(fvc::grad(this->U_)), KK);
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix> dynamicKEqn<BasicTurbulenceModel>::kSource() const
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


// Constructors 
template<class BasicTurbulenceModel>
dynamicKEqn<BasicTurbulenceModel>::dynamicKEqn
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
  k_
  {
    {
      IOobject::groupName("k", this->U_.group()),
      this->runTime_.timeName(),
      this->mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh_
  },
  simpleFilter_{this->mesh_},
  filterPtr_{LESfilter::New(this->mesh_, this->coeffDict())},
  filter_{filterPtr_()}
{
  bound(k_, this->kMin_);
  if (type == typeName) {
    correctNut();
    this->printCoeffs(type);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool dynamicKEqn<BasicTurbulenceModel>::read()
{
  if (LESeddyViscosity<BasicTurbulenceModel>::read()) {
    filter_.read(this->coeffDict());
    return true;
  }
  return false;
}


template<class BasicTurbulenceModel>
tmp<volScalarField> dynamicKEqn<BasicTurbulenceModel>::epsilon() const
{
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
        Ce()*k()*sqrt(k())/this->delta()
      }
    };
}


template<class BasicTurbulenceModel>
void dynamicKEqn<BasicTurbulenceModel>::correct()
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
  LESeddyViscosity<BasicTurbulenceModel>::correct();
  volScalarField divU{fvc::div(fvc::absolute(this->phi(), U))};
  tmp<volTensorField> tgradU{fvc::grad(U)};
  const volSymmTensorField D{dev(symm(tgradU()))};
  const volScalarField G{this->GName(), 2.0*nut*(tgradU() && D)};
  tgradU.clear();
  volScalarField KK{0.5*(filter_(magSqr(U)) - magSqr(filter_(U)))};
  KK.max(dimensionedScalar("small", KK.dimensions(), SMALL));
  tmp<fvScalarMatrix> kEqn {
    fvm::ddt(alpha, rho, k_)
  + fvm::div(alphaRhoPhi, k_)
  - fvm::laplacian(alpha*rho*DkEff(), k_)
  ==
    alpha*rho*G
  - fvm::SuSp((2.0/3.0)*alpha*rho*divU, k_)
  - fvm::Sp(Ce(D, KK)*alpha*rho*sqrt(k_)/this->delta(), k_)
  + kSource()
  };
  kEqn().relax();
  kEqn().solve();
  bound(k_, this->kMin_);
  correctNut(D, KK);
}

}  // namespace LESModels
}  // namespace mousse

