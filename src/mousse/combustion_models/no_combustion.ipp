// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_combustion.hpp"
#include "fvm_sup.hpp"


// Constructors 
template<class CombThermoType>
mousse::combustionModels::noCombustion<CombThermoType>::noCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  CombThermoType{modelType, mesh, phaseName}
{}


// Destructor 
template<class CombThermoType>
mousse::combustionModels::noCombustion<CombThermoType>::~noCombustion()
{}


// Member Functions 
template<class CombThermoType>
void mousse::combustionModels::noCombustion<CombThermoType>::correct()
{
//  Do Nothing
}


template<class CombThermoType>
mousse::tmp<mousse::fvScalarMatrix>
mousse::combustionModels::noCombustion<CombThermoType>::R
(
  volScalarField& Y
) const
{
  tmp<fvScalarMatrix> tSu{new fvScalarMatrix{Y, dimMass/dimTime}};
  return tSu;
}


template<class CombThermoType>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::noCombustion<CombThermoType>::dQ() const
{
  tmp<volScalarField> tdQ
  {
    new volScalarField
    {
      {
        IOobject::groupName("dQ", this->phaseName_),
        this->mesh().time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"dQ", dimEnergy/dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  return tdQ;
}


template<class CombThermoType>
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::noCombustion<CombThermoType>::Sh() const
{
  tmp<volScalarField> tSh
  {
    new volScalarField
    {
      {
        IOobject::groupName("Sh", this->phaseName_),
        this->mesh().time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimEnergy/dimTime/dimVolume, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  return tSh;
}


template<class CombThermoType>
bool mousse::combustionModels::noCombustion<CombThermoType>::read()
{
  if (CombThermoType::read()) {
    return true;
  }
  return false;
}

