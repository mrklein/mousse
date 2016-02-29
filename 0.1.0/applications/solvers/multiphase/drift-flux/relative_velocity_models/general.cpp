// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "general.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace relativeVelocityModels
{
DEFINE_TYPE_NAME_AND_DEBUG(general, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(relativeVelocityModel, general, dictionary);
}
}
// Constructors 
mousse::relativeVelocityModels::general::general
(
  const dictionary& dict,
  const incompressibleTwoPhaseInteractingMixture& mixture
)
:
  relativeVelocityModel{dict, mixture},
  a_{"a", dimless, dict},
  a1_{"a1", dimless, dict},
  V0_{"V0", dimVelocity, dict},
  residualAlpha_{dict.lookup("residualAlpha")}
{}
// Destructor 
mousse::relativeVelocityModels::general::~general()
{}
// Member Functions 
void mousse::relativeVelocityModels::general::correct()
{
  Udm_ =
    (rhoc_/rho())
    *V0_*(exp(-a_*max(alphad_ - residualAlpha_, scalar(0)))
          - exp(-a1_*max(alphad_ - residualAlpha_, scalar(0))));
}
