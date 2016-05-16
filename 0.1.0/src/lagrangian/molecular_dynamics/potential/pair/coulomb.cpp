// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coulomb.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace pairPotentials {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(coulomb, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  pairPotential,
  coulomb,
  dictionary
);


scalar coulomb::oneOverFourPiEps0 =
  1.0/(4.0*constant::mathematical::pi*8.854187817e-12);


// Constructors 
coulomb::coulomb
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
scalar coulomb::unscaledEnergy(const scalar r) const
{
  return oneOverFourPiEps0/r;
}


bool coulomb::read(const dictionary& pairPotentialProperties)
{
  pairPotential::read(pairPotentialProperties);
  return true;
}

}  // namespace pairPotentials
}  // namespace mousse

