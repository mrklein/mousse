// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction.hpp"
#include "reaction_types.hpp"
#include "langmuir_hinshelwood_reaction_rate.hpp"


namespace mousse {

MAKE_IR_REACTIONS(gasHThermoPhysics, LangmuirHinshelwoodReactionRate)
MAKE_IR_REACTIONS(icoPoly8HThermoPhysics, LangmuirHinshelwoodReactionRate)
MAKE_IR_REACTIONS(gasEThermoPhysics, LangmuirHinshelwoodReactionRate)
MAKE_IR_REACTIONS(icoPoly8EThermoPhysics, LangmuirHinshelwoodReactionRate)

}

