// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_chemistry_model.hpp"
#include "fv_mesh.hpp"
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(psiChemistryModel, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(psiChemistryModel, fvMesh);
}
// Constructors 
mousse::psiChemistryModel::psiChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicChemistryModel(mesh, phaseName),
  thermo_(psiReactionThermo::New(mesh, phaseName))
{}
// Selectors
mousse::autoPtr<mousse::psiChemistryModel> mousse::psiChemistryModel::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicChemistryModel::New<psiChemistryModel>(mesh, phaseName);
}
// Destructor 
mousse::psiChemistryModel::~psiChemistryModel()
{}
