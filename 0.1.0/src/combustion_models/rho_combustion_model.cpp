// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_combustion_model.hpp"
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
namespace mousse
{
namespace combustionModels
{
  defineTypeNameAndDebug(rhoCombustionModel, 0);
  defineRunTimeSelectionTable(rhoCombustionModel, dictionary);
}
}
// Constructors 
mousse::combustionModels::rhoCombustionModel::rhoCombustionModel
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  combustionModel(modelType, mesh, phaseName)
{}
// Destructor 
mousse::combustionModels::rhoCombustionModel::~rhoCombustionModel()
{}
// Member Functions 
bool mousse::combustionModels::rhoCombustionModel::read()
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
