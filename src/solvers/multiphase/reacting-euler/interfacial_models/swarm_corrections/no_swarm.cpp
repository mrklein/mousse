// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_swarm.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace swarmCorrections {
DEFINE_TYPE_NAME_AND_DEBUG(noSwarm, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(swarmCorrection, noSwarm, dictionary);
}
}
// Constructors 
mousse::swarmCorrections::noSwarm::noSwarm
(
  const dictionary& dict,
  const phasePair& pair
)
:
  swarmCorrection{dict, pair}
{}
// Destructor 
mousse::swarmCorrections::noSwarm::~noSwarm()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::swarmCorrections::noSwarm::Cs() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "one",
          mesh.time().timeName(),
          mesh
        },
        mesh,
        {"one", dimless, 1}
      }
    };
}
