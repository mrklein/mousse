// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction.hpp"
#include "reaction_types.hpp"
#include "langmuir_hinshelwood_reaction_rate.hpp"
namespace mousse
{
  makeIRReactions(gasHThermoPhysics, LangmuirHinshelwoodReactionRate)
  makeIRReactions(icoPoly8HThermoPhysics, LangmuirHinshelwoodReactionRate)
  makeIRReactions(gasEThermoPhysics, LangmuirHinshelwoodReactionRate)
  makeIRReactions(icoPoly8EThermoPhysics, LangmuirHinshelwoodReactionRate)
}
