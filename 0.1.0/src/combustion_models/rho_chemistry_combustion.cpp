// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_chemistry_combustion.hpp"
// Constructors 
mousse::combustionModels::rhoChemistryCombustion::rhoChemistryCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  rhoCombustionModel(modelType, mesh, phaseName),
  chemistryPtr_(rhoChemistryModel::New(mesh, phaseName))
{}
// Destructor 
mousse::combustionModels::rhoChemistryCombustion::~rhoChemistryCombustion()
{}
// Member Functions 
mousse::rhoReactionThermo&
mousse::combustionModels::rhoChemistryCombustion::thermo()
{
  return chemistryPtr_->thermo();
}
const mousse::rhoReactionThermo&
mousse::combustionModels::rhoChemistryCombustion::thermo() const
{
  return chemistryPtr_->thermo();
}
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::rhoChemistryCombustion::rho() const
{
  return chemistryPtr_->thermo().rho();
}
