// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_solid_reaction.hpp"
#include "solid_arrhenius_reaction_rate.hpp"
#include "solid_thermo_physics_types.hpp"
namespace mousse
{
// Make Solid reactions 
makeSolidIRReactions(hConstSolidThermoPhysics, solidArrheniusReactionRate)
makeSolidIRReactions
(
  hPowerSolidThermoPhysics,
  solidArrheniusReactionRate
)
makeSolidIRReactions
(
  hExpKappaConstSolidThermoPhysics,
  solidArrheniusReactionRate
)
}  // namespace mousse
