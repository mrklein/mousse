// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "simple.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace relativeVelocityModels {

DEFINE_TYPE_NAME_AND_DEBUG(simple, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(relativeVelocityModel, simple, dictionary);

}
}


// Constructors 
mousse::relativeVelocityModels::simple::simple
(
  const dictionary& dict,
  const incompressibleTwoPhaseInteractingMixture& mixture
)
:
  relativeVelocityModel{dict, mixture},
  a_{"a", dimless, dict},
  V0_{"V0", dimVelocity, dict},
  residualAlpha_{"residualAlpha", dimless, dict}
{}


// Destructor 
mousse::relativeVelocityModels::simple::~simple()
{}


// Member Functions 
void mousse::relativeVelocityModels::simple::correct()
{
  Udm_ = (rhoc_/rho())*V0_*pow(scalar(10), -a_*max(alphad_, scalar(0)));
}

