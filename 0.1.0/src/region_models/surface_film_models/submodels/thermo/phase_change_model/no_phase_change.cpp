// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_phase_change.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noPhaseChange, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(phaseChangeModel, noPhaseChange, dictionary);


// Constructors 
noPhaseChange::noPhaseChange
(
  surfaceFilmModel& owner,
  const dictionary&
)
:
  phaseChangeModel{owner}
{}


// Destructor 
noPhaseChange::~noPhaseChange()
{}


// Member Functions 
bool noPhaseChange::active() const
{
  return false;
}


void noPhaseChange::correctModel
(
  const scalar,
  scalarField&,
  scalarField&,
  scalarField&
)
{
  // do nothing
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

