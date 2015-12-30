// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_reaction_thermo.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
  defineTypeNameAndDebug(psiReactionThermo, 0);
  defineRunTimeSelectionTable(psiReactionThermo, fvMesh);
}
// Constructors 
mousse::psiReactionThermo::psiReactionThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  psiThermo(mesh, phaseName)
{}
// Selectors
mousse::autoPtr<mousse::psiReactionThermo> mousse::psiReactionThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<psiReactionThermo>(mesh, phaseName);
}
// Destructor 
mousse::psiReactionThermo::~psiReactionThermo()
{}
