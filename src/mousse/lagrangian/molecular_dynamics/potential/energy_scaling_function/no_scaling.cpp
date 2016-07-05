// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_scaling.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace energyScalingFunctions {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noScaling, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  energyScalingFunction,
  noScaling,
  dictionary
);


// Constructors 
noScaling::noScaling
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  energyScalingFunction{name, energyScalingFunctionProperties, pairPot}
{}


// Member Functions 
void noScaling::scaleEnergy(scalar& /*e*/, const scalar /*r*/) const
{}


bool noScaling::read(const dictionary& energyScalingFunctionProperties)
{
  energyScalingFunction::read(energyScalingFunctionProperties);
  return true;
}

}  // namespace energyScalingFunctions
}  // namespace mousse

