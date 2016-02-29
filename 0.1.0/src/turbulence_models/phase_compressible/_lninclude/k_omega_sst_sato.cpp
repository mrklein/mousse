// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "k_omega_sst_sato.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "two_phase_system.hpp"
#include "drag_model.hpp"
namespace mousse
{
namespace RASModels
{
// Constructors 
template<class BasicTurbulenceModel>
kOmegaSSTSato<BasicTurbulenceModel>::kOmegaSSTSato
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
  kOmegaSST<BasicTurbulenceModel>
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
  gasTurbulencePtr_(NULL),
  Cmub_
  (
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Cmub",
      this->coeffDict_,
      0.6
    )
  )
{
  if (type == typeName)
  {
    // Cannot correct nut yet: construction of the phases is not complete
    // correctNut();
    this->printCoeffs(type);
  }
}
// Member Functions 
template<class BasicTurbulenceModel>
bool kOmegaSSTSato<BasicTurbulenceModel>::read()
{
  if (kOmegaSST<BasicTurbulenceModel>::read())
  {
    Cmub_.readIfPresent(this->coeffDict());
    return true;
  }
  else
  {
    return false;
  }
}
template<class BasicTurbulenceModel>
const PhaseCompressibleTurbulenceModel
<
  typename BasicTurbulenceModel::transportModel
>&
kOmegaSSTSato<BasicTurbulenceModel>::gasTurbulence() const
{
  if (!gasTurbulencePtr_)
  {
    const volVectorField& U = this->U_;
    const transportModel& liquid = this->transport();
    const twoPhaseSystem& fluid =
      refCast<const twoPhaseSystem>(liquid.fluid());
    const transportModel& gas = fluid.otherPhase(liquid);
    gasTurbulencePtr_ =
     &U.db()
     .lookupObject<PhaseCompressibleTurbulenceModel<transportModel> >
      (
        IOobject::groupName
        (
          turbulenceModel::propertiesName,
          gas.name()
        )
      );
  }
  return *gasTurbulencePtr_;
}
template<class BasicTurbulenceModel>
void kOmegaSSTSato<BasicTurbulenceModel>::correctNut()
{
  const PhaseCompressibleTurbulenceModel<transportModel>& gasTurbulence =
    this->gasTurbulence();
  volScalarField yPlus
  (
    pow(this->betaStar_, 0.25)*this->y_*sqrt(this->k_)/this->nu()
  );
  this->nut_ =
    this->a1_*this->k_
   /max
    (
      this->a1_*this->omega_,
      this->F23()*sqrt(2.0)*mag(symm(fvc::grad(this->U_)))
    )
   + sqr(1 - exp(-yPlus/16.0))
   *Cmub_*gasTurbulence.transport().d()*gasTurbulence.alpha()
   *(mag(this->U_ - gasTurbulence.U()));
  this->nut_.correctBoundaryConditions();
}
template<class BasicTurbulenceModel>
void kOmegaSSTSato<BasicTurbulenceModel>::correct()
{
  kOmegaSST<BasicTurbulenceModel>::correct();
}
}  // namespace RASModels
}  // namespace mousse
