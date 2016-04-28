// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "buoyant_k_epsilon.hpp"
#include "uniform_dimensioned_fields.hpp"
#include "fvc_grad.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace RASModels {

// Constructors 
template<class BasicTurbulenceModel>
buoyantKEpsilon<BasicTurbulenceModel>::buoyantKEpsilon
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
  kEpsilon<BasicTurbulenceModel>
  {
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    transport,
    propertiesName,
    type
  },
  Cg_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "Cg",
      this->coeffDict_,
      1.0
    )
  }
{
  if (type == typeName) {
    kEpsilon<BasicTurbulenceModel>::correctNut();
    this->printCoeffs(type);
  }
}


// Member Functions 
template<class BasicTurbulenceModel>
bool buoyantKEpsilon<BasicTurbulenceModel>::read()
{
  if (kEpsilon<BasicTurbulenceModel>::read()) {
    Cg_.readIfPresent(this->coeffDict());
    return true;
  } else {
    return false;
  }
}


template<class BasicTurbulenceModel>
tmp<volScalarField>
buoyantKEpsilon<BasicTurbulenceModel>::Gcoef() const
{
  const uniformDimensionedVectorField& g =
    this->mesh_.objectRegistry::template
    lookupObject<uniformDimensionedVectorField>("g");
  return
    (Cg_*this->Cmu_)*this->alpha_*this->k_*(g & fvc::grad(this->rho_))
    /(this->epsilon_ + this->epsilonMin_);
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix>
buoyantKEpsilon<BasicTurbulenceModel>::kSource() const
{
  const uniformDimensionedVectorField& g =
    this->mesh_.objectRegistry::template
    lookupObject<uniformDimensionedVectorField>("g");
  if (mag(g.value()) > SMALL) {
    return -fvm::SuSp(Gcoef(), this->k_);
  } else {
    return kEpsilon<BasicTurbulenceModel>::kSource();
  }
}


template<class BasicTurbulenceModel>
tmp<fvScalarMatrix>
buoyantKEpsilon<BasicTurbulenceModel>::epsilonSource() const
{
  const uniformDimensionedVectorField& g =
    this->mesh_.objectRegistry::template
    lookupObject<uniformDimensionedVectorField>("g");
  if (mag(g.value()) > SMALL) {
    vector gHat{g.value()/mag(g.value())};
    volScalarField v{gHat & this->U_};
    volScalarField u
    {
      mag(this->U_ - gHat*v)
      + dimensionedScalar("SMALL", dimVelocity, SMALL)
    };
    return -fvm::SuSp(this->C1_*tanh(mag(v)/u)*Gcoef(), this->epsilon_);
  } else {
    return kEpsilon<BasicTurbulenceModel>::epsilonSource();
  }
}

}  // namespace RASModels
}  // namespace mousse

