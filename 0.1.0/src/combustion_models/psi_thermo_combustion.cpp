// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_thermo_combustion.hpp"


// Constructors 
mousse::combustionModels::psiThermoCombustion::psiThermoCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  psiCombustionModel{modelType, mesh, phaseName},
  thermoPtr_{psiReactionThermo::New(mesh, phaseName)}
{}


// Destructor 
mousse::combustionModels::psiThermoCombustion::~psiThermoCombustion()
{}


// Member Functions 
mousse::psiReactionThermo&
mousse::combustionModels::psiThermoCombustion::thermo()
{
  return thermoPtr_();
}


const mousse::psiReactionThermo&
mousse::combustionModels::psiThermoCombustion::thermo() const
{
  return thermoPtr_();
}


mousse::tmp<mousse::volScalarField>
mousse::combustionModels::psiThermoCombustion::rho() const
{
  return thermoPtr_->rho();
}

