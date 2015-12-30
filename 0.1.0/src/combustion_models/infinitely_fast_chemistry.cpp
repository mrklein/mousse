// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "infinitely_fast_chemistry.hpp"
namespace mousse
{
namespace combustionModels
{
// Constructors 
template<class CombThermoType, class ThermoType>
infinitelyFastChemistry<CombThermoType, ThermoType>::infinitelyFastChemistry
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  singleStepCombustion<CombThermoType, ThermoType>
  (
    modelType,
    mesh,
    phaseName
  ),
  C_(readScalar(this->coeffs().lookup("C")))
{}
// Destructor 
template<class CombThermoType, class ThermoType>
infinitelyFastChemistry<CombThermoType, ThermoType>::~infinitelyFastChemistry()
{}
// Member Functions 
template<class CombThermoType, class ThermoType>
void infinitelyFastChemistry<CombThermoType, ThermoType>::correct()
{
  this->wFuel_ ==
    dimensionedScalar("zero", dimMass/pow3(dimLength)/dimTime, 0.0);
  if (this->active())
  {
    this->singleMixturePtr_->fresCorrect();
    const label fuelI = this->singleMixturePtr_->fuelIndex();
    const volScalarField& YFuel =
      this->thermoPtr_->composition().Y()[fuelI];
    const dimensionedScalar s = this->singleMixturePtr_->s();
    if (this->thermoPtr_->composition().contains("O2"))
    {
      const volScalarField& YO2 = this->thermoPtr_->composition().Y("O2");
      this->wFuel_ ==
        this->rho()/(this->mesh().time().deltaT()*C_)
       *min(YFuel, YO2/s.value());
    }
  }
}
template<class CombThermoType, class ThermoType>
bool infinitelyFastChemistry<CombThermoType, ThermoType>::read()
{
  if (singleStepCombustion<CombThermoType, ThermoType>::read())
  {
    this->coeffs().lookup("C") >> C_ ;
    return true;
  }
  else
  {
    return false;
  }
}
}  // namespace combustionModels
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
