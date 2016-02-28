// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_swarm.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace swarmCorrections {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaSwarm, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  swarmCorrection,
  TomiyamaSwarm,
  dictionary
);
}
}
// Constructors 
mousse::swarmCorrections::TomiyamaSwarm::TomiyamaSwarm
(
  const dictionary& dict,
  const phasePair& pair
)
:
  swarmCorrection{dict, pair},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    dict.lookupOrDefault<scalar>
    (
      "residualAlpha",
      pair_.dispersed().residualAlpha().value()
    )
  },
  l_{"l", dimless, dict}
{}
// Destructor 
mousse::swarmCorrections::TomiyamaSwarm::~TomiyamaSwarm()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::swarmCorrections::TomiyamaSwarm::Cs() const
{
  return pow(max(this->pair_.continuous(), residualAlpha_), scalar(3) - 2*l_);
}
