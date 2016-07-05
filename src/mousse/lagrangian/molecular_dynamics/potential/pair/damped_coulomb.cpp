// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "damped_coulomb.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace pairPotentials {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(dampedCoulomb, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  pairPotential,
  dampedCoulomb,
  dictionary
);


scalar dampedCoulomb::oneOverFourPiEps0 =
  1.0/(4.0*constant::mathematical::pi*8.854187817e-12);


// Constructors 
dampedCoulomb::dampedCoulomb
(
  const word& name,
  const dictionary& pairPotentialProperties
)
:
  pairPotential{name, pairPotentialProperties},
  dampedCoulombCoeffs_
  {
    pairPotentialProperties.subDict(typeName + "Coeffs")
  },
  alpha_{readScalar(dampedCoulombCoeffs_.lookup("alpha"))}
{
  setLookupTables();
}


// Member Functions 
scalar dampedCoulomb::unscaledEnergy(const scalar r) const
{
  return oneOverFourPiEps0*erfc(alpha_*r)/r;
}


bool dampedCoulomb::read(const dictionary& pairPotentialProperties)
{
  pairPotential::read(pairPotentialProperties);
  dampedCoulombCoeffs_ =
    pairPotentialProperties.subDict(typeName + "Coeffs");
  dampedCoulombCoeffs_.lookup("alpha") >> alpha_;
  return true;
}

}  // namespace pairPotentials
}  // namespace mousse

