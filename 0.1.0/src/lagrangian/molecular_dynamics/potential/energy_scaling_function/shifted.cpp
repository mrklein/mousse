// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "shifted.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace energyScalingFunctions
{
// Static Data Members
defineTypeNameAndDebug(shifted, 0);
addToRunTimeSelectionTable
(
  energyScalingFunction,
  shifted,
  dictionary
);
// Constructors 
shifted::shifted
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  energyScalingFunction(name, energyScalingFunctionProperties, pairPot),
  e_at_rCut_(pairPot.unscaledEnergy(pairPot.rCut()))
{}
// Member Functions 
void shifted::scaleEnergy(scalar& e, const scalar r) const
{
  e -= e_at_rCut_;
}
bool shifted::read(const dictionary& energyScalingFunctionProperties)
{
  energyScalingFunction::read(energyScalingFunctionProperties);
  return true;
}
}  // namespace energyScalingFunctions
}  // namespace mousse
