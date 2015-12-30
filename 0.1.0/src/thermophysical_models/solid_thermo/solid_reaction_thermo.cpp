// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_reaction_thermo.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
  defineTypeNameAndDebug(solidReactionThermo, 0);
  defineRunTimeSelectionTable(solidReactionThermo, fvMesh);
  defineRunTimeSelectionTable(solidReactionThermo, dictionary);
}
// Constructors 
mousse::solidReactionThermo::solidReactionThermo
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  solidThermo(mesh, phaseName)
{}
mousse::solidReactionThermo::solidReactionThermo
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
:
  solidThermo(mesh, dict, phaseName)
{}
// Selectors
mousse::autoPtr<mousse::solidReactionThermo> mousse::solidReactionThermo::New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  return basicThermo::New<solidReactionThermo>(mesh, phaseName);
}
mousse::autoPtr<mousse::solidReactionThermo> mousse::solidReactionThermo::New
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& phaseName
)
{
  return basicThermo::New<solidReactionThermo>(mesh, dict, phaseName);
}
// Destructor 
mousse::solidReactionThermo::~solidReactionThermo()
{}
