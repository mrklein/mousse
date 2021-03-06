// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "energy_scaling_function.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(energyScalingFunction, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(energyScalingFunction, dictionary);

}


// Constructors 
mousse::energyScalingFunction::energyScalingFunction
(
  const word& name,
  const dictionary& energyScalingFunctionProperties,
  const pairPotential& pairPot
)
:
  name_{name},
  energyScalingFunctionProperties_{energyScalingFunctionProperties},
  pairPot_{pairPot}
{}


// Member Functions 
bool mousse::energyScalingFunction::read
(
  const dictionary& energyScalingFunctionProperties
)
{
  energyScalingFunctionProperties_ = energyScalingFunctionProperties;
  return true;
}

