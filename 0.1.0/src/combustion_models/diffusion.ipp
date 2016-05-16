// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "diffusion.hpp"
#include "fvc_grad.hpp"


namespace mousse {
namespace combustionModels {

// Constructors 
template<class CombThermoType, class ThermoType>
diffusion<CombThermoType, ThermoType>::diffusion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  singleStepCombustion<CombThermoType, ThermoType>
  {
    modelType,
    mesh,
    phaseName
  },
  C_{readScalar(this->coeffs().lookup("C"))},
  oxidantName_{this->coeffs().template lookupOrDefault<word>("oxidant", "O2")}
{}


// Destructor 
template<class CombThermoType, class ThermoType>
diffusion<CombThermoType, ThermoType>::~diffusion()
{}


// Member Functions 
template<class CombThermoType, class ThermoType>
void diffusion<CombThermoType, ThermoType>::correct()
{
  this->wFuel_ ==
    dimensionedScalar{"zero", dimMass/pow3(dimLength)/dimTime, 0.0};
  if (this->active()) {
    this->singleMixturePtr_->fresCorrect();
    const label fuelI = this->singleMixturePtr_->fuelIndex();
    const volScalarField& YFuel =
      this->thermoPtr_->composition().Y()[fuelI];
    if (this->thermoPtr_->composition().contains(oxidantName_)) {
      const volScalarField& YO2 =
        this->thermoPtr_->composition().Y(oxidantName_);
      this->wFuel_ ==
        C_*this->turbulence().muEff()
        *mag(fvc::grad(YFuel) & fvc::grad(YO2))
        *pos(YFuel)*pos(YO2);
    }
  }
}


template<class CombThermoType, class ThermoType>
bool diffusion<CombThermoType, ThermoType>::read()
{
  if (singleStepCombustion<CombThermoType, ThermoType>::read()) {
    this->coeffs().lookup("C") >> C_ ;
    this->coeffs().readIfPresent("oxidant", oxidantName_);
    return true;
  }
  return false;
}

}  // namespace combustionModels
}  // namespace mousse

