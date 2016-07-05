// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_phase_model.hpp"
#include "phase_system.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "fvm_laplacian.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
// Constructors 
template<class BasePhaseModel, class ThermoType>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::ThermoPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index,
  const bool createThermo
)
:
  BasePhaseModel(fluid, phaseName, index)
{
  if (createThermo)
  {
    thermoPtr_.set
    (
      ThermoType::New(fluid.mesh(), this->name()).ptr()
    );
    thermo_ = thermoPtr_.ptr();
    thermo_->validate
    (
      IOobject::groupName(phaseModel::typeName, this->name()),
      "h",
      "e"
    );
  }
}
// Destructor 
template<class BasePhaseModel, class ThermoType>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::~ThermoPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel, class ThermoType>
const mousse::rhoThermo&
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::thermo() const
{
  return *thermo_;
}
template<class BasePhaseModel, class ThermoType>
mousse::rhoThermo&
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::thermo()
{
  return *thermo_;
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::rho() const
{
  return thermo_->rho();
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::mu() const
{
  return thermo_->mu();
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::mu
(
  const label patchi
) const
{
  return thermo_->mu(patchi);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::nu() const
{
  return thermo_->nu();
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::nu
(
  const label patchi
) const
{
  return thermo_->nu(patchi);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::kappa() const
{
  return thermo_->kappa();
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::kappa
(
  const label patchi
) const
{
  return thermo_->kappa(patchi);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::kappaEff
(
  const volScalarField& alphat
) const
{
  return thermo_->kappaEff(alphat);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::kappaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return thermo_->kappaEff(alphat, patchi);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::alpha() const
{
  return thermo_->alpha();
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::alpha
(
  const label patchi
) const
{
  return thermo_->alpha(patchi);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::alphaEff
(
  const volScalarField& alphat
) const
{
  return thermo_->alphaEff(alphat);
}
template<class BasePhaseModel, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::ThermoPhaseModel<BasePhaseModel, ThermoType>::alphaEff
(
  const scalarField& alphat,
  const label patchi
) const
{
  return thermo_->alphaEff(alphat, patchi);
}
