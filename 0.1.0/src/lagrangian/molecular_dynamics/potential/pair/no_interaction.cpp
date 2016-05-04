// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_interaction.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace pairPotentials {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noInteraction, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  pairPotential,
  noInteraction,
  dictionary
);


// Constructors 
noInteraction::noInteraction
(
  const word& name,
  const dictionary& pairPotentialProperties
)
:
  pairPotential{name, pairPotentialProperties}
{
  setLookupTables();
}


// Member Functions 
scalar noInteraction::unscaledEnergy(const scalar /*r*/) const
{
  return 0.0;
}


bool noInteraction::read(const dictionary& pairPotentialProperties)
{
  pairPotential::read(pairPotentialProperties);
  return true;
}


}  // namespace pairPotentials
}  // namespace mousse

