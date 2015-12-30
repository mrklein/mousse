// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "continuous_gas_k_eqn.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "two_phase_system.hpp"
namespace mousse
{
namespace LESModels
{
// Constructors 
template<class BasicTurbulenceModel>
continuousGasKEqn<BasicTurbulenceModel>::continuousGasKEqn
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
  kEqn<BasicTurbulenceModel>
  (
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName,
    type
  ),
  liquidTurbulencePtr_(NULL),
  alphaInversion_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "alphaInversion",
      this->coeffDict_,
      0.7
    )
  )
{
  if (type == typeName)
  {
    kEqn<BasicTurbulenceModel>::correctNut();
    this->printCoeffs(type);
  }
}
// Member Functions 
template<class BasicTurbulenceModel>
bool continuousGasKEqn<BasicTurbulenceModel>::read()
{
  if (kEqn<BasicTurbulenceModel>::read())
  {
    alphaInversion_.readIfPresent(this->coeffDict());
    return true;
  }
  else
  {
    return false;
  }
}
template<class BasicTurbulenceModel>
const turbulenceModel&
continuousGasKEqn<BasicTurbulenceModel>::liquidTurbulence() const
{
  if (!liquidTurbulencePtr_)
  {
    const volVectorField& U = this->U_;
    const transportModel& gas = this->transport();
    const twoPhaseSystem& fluid =
      refCast<const twoPhaseSystem>(gas.fluid());
    const transportModel& liquid = fluid.otherPhase(gas);
    liquidTurbulencePtr_ =
     &U.db().lookupObject<turbulenceModel>
      (
        IOobject::groupName
        (
          turbulenceModel::propertiesName,
          liquid.name()
        )
      );
  }
  return *liquidTurbulencePtr_;
}
template<class BasicTurbulenceModel>
tmp<volScalarField>
continuousGasKEqn<BasicTurbulenceModel>::phaseTransferCoeff() const
{
  const volVectorField& U = this->U_;
  const alphaField& alpha = this->alpha_;
  const rhoField& rho = this->rho_;
  const turbulenceModel& liquidTurbulence = this->liquidTurbulence();
  return
  (
    max(alphaInversion_ - alpha, scalar(0))
   *rho
   *min
    (
      this->Ce_*sqrt(liquidTurbulence.k())/this->delta(),
      1.0/U.time().deltaT()
    )
  );
}
template<class BasicTurbulenceModel>
tmp<fvScalarMatrix>
continuousGasKEqn<BasicTurbulenceModel>::kSource() const
{
  const turbulenceModel& liquidTurbulence = this->liquidTurbulence();
  const volScalarField phaseTransferCoeff(this->phaseTransferCoeff());
  return
    phaseTransferCoeff*liquidTurbulence.k()
   - fvm::Sp(phaseTransferCoeff, this->k_);
}
}  // namespace LESModels
}  // namespace mousse
