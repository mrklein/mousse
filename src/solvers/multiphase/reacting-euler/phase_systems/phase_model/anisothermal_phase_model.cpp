// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "anisothermal_phase_model.hpp"
#include "phase_system.hpp"
// Constructors 
template<class BasePhaseModel>
mousse::AnisothermalPhaseModel<BasePhaseModel>::AnisothermalPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel(fluid, phaseName, index),
  divU_(NULL),
  K_
  (
    IOobject
    (
      IOobject::groupName("K", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    ),
    fluid.mesh(),
    dimensionedScalar("K", sqr(dimVelocity), scalar(0))
  )
{}
// Destructor 
template<class BasePhaseModel>
mousse::AnisothermalPhaseModel<BasePhaseModel>::~AnisothermalPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
void mousse::AnisothermalPhaseModel<BasePhaseModel>::correctKinematics()
{
  BasePhaseModel::correctKinematics();
  K_ = 0.5*magSqr(this->U());
}
template<class BasePhaseModel>
void mousse::AnisothermalPhaseModel<BasePhaseModel>::correctThermo()
{
  BasePhaseModel::correctThermo();
  this->thermo_->correct();
}
template<class BasePhaseModel>
mousse::tmp<mousse::fvScalarMatrix>
mousse::AnisothermalPhaseModel<BasePhaseModel>::heEqn()
{
  const volScalarField& alpha = *this;
  const surfaceScalarField& alphaPhi = this->alphaPhi();
  const surfaceScalarField& alphaRhoPhi = this->alphaRhoPhi();
  const volScalarField& contErr(this->continuityError());
  const volScalarField alphaEff(this->turbulence().alphaEff());
  volScalarField& he = this->thermo_->he();
  tmp<fvScalarMatrix> tEEqn
  (
    fvm::ddt(alpha, this->rho(), he) + fvm::div(alphaRhoPhi, he)
   - fvm::Sp(contErr, he)
   + fvc::ddt(alpha, this->rho(), K_) + fvc::div(alphaRhoPhi, K_)
   - contErr*K_
   - fvm::laplacian
    (
      fvc::interpolate(alpha)
     *fvc::interpolate(alphaEff),
      he
    )
  ==
    this->Sh()
  );
  // Add the appropriate pressure-work term
  if (he.name() == this->thermo_->phasePropertyName("e"))
  {
    tEEqn() +=
      fvc::ddt(alpha)*this->thermo().p()
     + fvc::div(alphaPhi, this->thermo().p());
  }
  else if (this->thermo_->dpdt())
  {
    tEEqn() -= alpha*this->fluid().dpdt();
  }
  return tEEqn;
}
template<class BasePhaseModel>
bool mousse::AnisothermalPhaseModel<BasePhaseModel>::compressible() const
{
  return !this->thermo().incompressible();
}
template<class BasePhaseModel>
const mousse::tmp<mousse::volScalarField>&
mousse::AnisothermalPhaseModel<BasePhaseModel>::divU() const
{
  return divU_;
}
template<class BasePhaseModel>
void
mousse::AnisothermalPhaseModel<BasePhaseModel>::divU
(
  const tmp<volScalarField>& divU
)
{
  divU_ = divU;
}
template<class BasePhaseModel>
const mousse::volScalarField&
mousse::AnisothermalPhaseModel<BasePhaseModel>::K() const
{
  return K_;
}
