// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_combustion_model.hpp"
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
namespace mousse
{
namespace combustionModels
{
  defineTypeNameAndDebug(psiCombustionModel, 0);
  defineRunTimeSelectionTable(psiCombustionModel, dictionary);
}
}
// Constructors 
mousse::combustionModels::psiCombustionModel::psiCombustionModel
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  combustionModel(modelType, mesh, phaseName)
{}
// Destructor 
mousse::combustionModels::psiCombustionModel::~psiCombustionModel()
{}
// Member Functions 
bool mousse::combustionModels::psiCombustionModel::read()
{
  if (combustionModel::read())
  {
    return true;
  }
  else
  {
    return false;
  }
}
