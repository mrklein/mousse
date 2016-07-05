// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "swarm_correction.hpp"
#include "phase_pair.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(swarmCorrection, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(swarmCorrection, dictionary);
}
// Constructors 
mousse::swarmCorrection::swarmCorrection
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  pair_{pair}
{}
// Destructor 
mousse::swarmCorrection::~swarmCorrection()
{}
