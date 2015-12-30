// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_chemistry_combustion.hpp"
// Constructors 
mousse::combustionModels::psiChemistryCombustion::psiChemistryCombustion
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  psiCombustionModel(modelType, mesh, phaseName),
  chemistryPtr_(psiChemistryModel::New(mesh, phaseName))
{}
// Destructor 
mousse::combustionModels::psiChemistryCombustion::~psiChemistryCombustion()
{}
// Member Functions 
mousse::psiReactionThermo&
mousse::combustionModels::psiChemistryCombustion::thermo()
{
  return chemistryPtr_->thermo();
}
const mousse::psiReactionThermo&
mousse::combustionModels::psiChemistryCombustion::thermo() const
{
  return chemistryPtr_->thermo();
}
mousse::tmp<mousse::volScalarField>
mousse::combustionModels::psiChemistryCombustion::rho() const
{
  return chemistryPtr_->thermo().rho();
}
