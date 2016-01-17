// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_reaction_thermo.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(rhoReactionThermo, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(rhoReactionThermo, fvMesh);
}
// Constructors 
mousse::rhoReactionThermo::rhoReactionThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  rhoThermo(mesh, phaseName)
{}
// Selectors
mousse::autoPtr<mousse::rhoReactionThermo> mousse::rhoReactionThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<rhoReactionThermo>(mesh, phaseName);
}
// Destructor 
mousse::rhoReactionThermo::~rhoReactionThermo()
{}
