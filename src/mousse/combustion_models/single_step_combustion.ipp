// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "single_step_combustion.hpp"
#include "fvm_sup.hpp"


namespace mousse {
namespace combustionModels {

// Constructors 
template<class CombThermoType, class ThermoType>
singleStepCombustion<CombThermoType, ThermoType>::singleStepCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  CombThermoType{modelType, mesh, phaseName},
  singleMixturePtr_{NULL},
  wFuel_
  {
    IOobject
    {
      IOobject::groupName("wFuel", phaseName),
      this->mesh().time().timeName(),
      this->mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    this->mesh(),
    {"zero", dimMass/dimVolume/dimTime, 0.0}
  },
  semiImplicit_{readBool(this->coeffs_.lookup("semiImplicit"))}
{
  if (isA<singleStepReactingMixture<ThermoType> >(this->thermo())) {
    singleMixturePtr_ =
      &dynamic_cast<singleStepReactingMixture<ThermoType>&>
      (
        this->thermo()
      );
  } else {
    FATAL_ERROR_IN
    (
      "singleStepCombustion<CombThermoType, ThermoType>::"
      "singleStepCombustion"
      "("
        "const word&, "
        "const fvMesh& "
        "const word&"
      ")"
    )
    << "Inconsistent thermo package for " << this->type() << " model:\n"
    << "    " << this->thermo().type() << nl << nl
    << "Please select a thermo package based on "
    << "singleStepReactingMixture" << exit(FatalError);
  }
  if (semiImplicit_) {
    Info << "Combustion mode: semi-implicit" << endl;
  } else {
    Info << "Combustion mode: explicit" << endl;
  }
}


// Destructor 
template<class CombThermoType, class ThermoType>
singleStepCombustion<CombThermoType, ThermoType>::~singleStepCombustion()
{}


// Member Functions 
template<class CombThermoType, class ThermoType>
tmp<fvScalarMatrix> singleStepCombustion<CombThermoType, ThermoType>::R
(
  volScalarField& Y
) const
{
  const label specieI =
    this->thermoPtr_->composition().species()[Y.member()];
  volScalarField wSpecie
  {
    wFuel_*singleMixturePtr_->specieStoichCoeffs()[specieI]
  };
  if (semiImplicit_) {
    const label fNorm = singleMixturePtr_->specieProd()[specieI];
    const volScalarField fres(singleMixturePtr_->fres(specieI));
    wSpecie /= max(fNorm*(Y - fres), scalar(1e-2));
    return -fNorm*wSpecie*fres + fNorm*fvm::Sp(wSpecie, Y);
  } else {
    return wSpecie + fvm::Sp(0.0*wSpecie, Y);
  }
}


template<class CombThermoType, class ThermoType>
tmp<volScalarField>
singleStepCombustion<CombThermoType, ThermoType>::Sh() const
{
  const label fuelI = singleMixturePtr_->fuelIndex();
  volScalarField& YFuel =
    const_cast<volScalarField&>(this->thermoPtr_->composition().Y(fuelI));
  return -singleMixturePtr_->qFuel()*(R(YFuel) & YFuel);
}


template<class CombThermoType, class ThermoType>
tmp<volScalarField>
singleStepCombustion<CombThermoType, ThermoType>::dQ() const
{
  tmp<volScalarField> tdQ
  {
    new volScalarField
    {
      {
        IOobject::groupName("dQ", this->phaseName_),
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh_,
      {"dQ", dimEnergy/dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  if (this->active()) {
    volScalarField& dQ = tdQ();
    dQ.dimensionedInternalField() = this->mesh().V()*Sh()();
  }
  return tdQ;
}


template<class CombThermoType, class ThermoType>
bool singleStepCombustion<CombThermoType, ThermoType>::read()
{
  if (CombThermoType::read()) {
    return true;
  } else {
    return false;
  }
}

}  // namespace combustionModels
}  // namespace mousse

