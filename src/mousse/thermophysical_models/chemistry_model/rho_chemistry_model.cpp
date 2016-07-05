// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_chemistry_model.hpp"
#include "fv_mesh.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(rhoChemistryModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(rhoChemistryModel, fvMesh);

}


// Constructors 

mousse::rhoChemistryModel::rhoChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicChemistryModel{mesh, phaseName},
  thermo_{rhoReactionThermo::New(mesh, phaseName)}
{}


// Selectors
mousse::autoPtr<mousse::rhoChemistryModel> mousse::rhoChemistryModel::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicChemistryModel::New<rhoChemistryModel>(mesh, phaseName);
}


// Destructor 
mousse::rhoChemistryModel::~rhoChemistryModel()
{}

