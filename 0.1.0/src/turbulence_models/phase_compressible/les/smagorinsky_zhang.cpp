// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "smagorinsky_zhang.hpp"
namespace mousse
{
namespace LESModels
{
// Constructors 
template<class BasicTurbulenceModel>
SmagorinskyZhang<BasicTurbulenceModel>::SmagorinskyZhang
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
  Smagorinsky<BasicTurbulenceModel>
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
bool SmagorinskyZhang<BasicTurbulenceModel>::read()
{
  if (Smagorinsky<BasicTurbulenceModel>::read())
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
SmagorinskyZhang<BasicTurbulenceModel>::gasTurbulence() const
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
void SmagorinskyZhang<BasicTurbulenceModel>::correctNut()
{
  const PhaseCompressibleTurbulenceModel<transportModel>& gasTurbulence =
    this->gasTurbulence();
  volScalarField k(this->k(fvc::grad(this->U_)));
  this->nut_ =
    this->Ck_*sqrt(k)*this->delta()
   + Cmub_*gasTurbulence.transport().d()*gasTurbulence.alpha()
   *(mag(this->U_ - gasTurbulence.U()));
  this->nut_.correctBoundaryConditions();
}
}  // namespace LESModels
}  // namespace mousse
