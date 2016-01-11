// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_solid_reaction.hpp"
#include "solid_arrhenius_reaction_rate.hpp"
#include "solid_thermo_physics_types.hpp"
namespace mousse
{
// Make Solid reactions 
MAKE_SOLID_IR_REACTIONS(hConstSolidThermoPhysics, solidArrheniusReactionRate)
MAKE_SOLID_IR_REACTIONS
(
  hPowerSolidThermoPhysics,
  solidArrheniusReactionRate
)
MAKE_SOLID_IR_REACTIONS
(
  hExpKappaConstSolidThermoPhysics,
  solidArrheniusReactionRate
)
}  // namespace mousse
