// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "shifted_force.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace energyScalingFunctions {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(shiftedForce, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  energyScalingFunction,
  shiftedForce,
  dictionary
);


// Constructors 
shiftedForce::shiftedForce
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  energyScalingFunction{name, energyScalingFunctionProperties, pairPot},
  rCut_{pairPot.rCut()},
  e_at_rCut_{pairPot.unscaledEnergy(rCut_)},
  de_dr_at_rCut_{pairPot.energyDerivative(rCut_, false)}
{}


// Member Functions 
void shiftedForce::scaleEnergy(scalar& e, const scalar r) const
{
  e -= ( e_at_rCut_ + de_dr_at_rCut_ * (r - rCut_) );
}


bool shiftedForce::read(const dictionary& energyScalingFunctionProperties)
{
  energyScalingFunction::read(energyScalingFunctionProperties);
  return true;
}

}  // namespace energyScalingFunctions
}  // namespace mousse

