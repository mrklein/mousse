// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_thermo_combustion.hpp"


// Constructors 
mousse::combustionModels::rhoThermoCombustion::rhoThermoCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  rhoCombustionModel{modelType, mesh, phaseName},
  thermoPtr_{rhoReactionThermo::New(mesh, phaseName)}
{}


// Destructor 
mousse::combustionModels::rhoThermoCombustion::~rhoThermoCombustion()
{}


// Member Functions 
mousse::rhoReactionThermo&
mousse::combustionModels::rhoThermoCombustion::thermo()
{
  return thermoPtr_();
}


const mousse::rhoReactionThermo&
mousse::combustionModels::rhoThermoCombustion::thermo() const
{
  return thermoPtr_();
}


mousse::tmp<mousse::volScalarField>
mousse::combustionModels::rhoThermoCombustion::rho() const
{
  return thermoPtr_().rho();
}

