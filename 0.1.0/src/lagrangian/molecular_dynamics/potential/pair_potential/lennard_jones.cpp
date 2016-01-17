// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lennard_jones.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace pairPotentials
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(lennardJones, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  pairPotential,
  lennardJones,
  dictionary
);
// Constructors 
lennardJones::lennardJones
(
  const word& name,
  const dictionary& pairPotentialProperties
)
:
  pairPotential(name, pairPotentialProperties),
  lennardJonesCoeffs_(pairPotentialProperties.subDict(typeName + "Coeffs")),
  sigma_(readScalar(lennardJonesCoeffs_.lookup("sigma"))),
  epsilon_(readScalar(lennardJonesCoeffs_.lookup("epsilon")))
{
  setLookupTables();
}
// Member Functions 
scalar lennardJones::unscaledEnergy(const scalar r) const
{
  // (rIJ/sigma)^-2
  scalar ir2 = (sigma_/r)*(sigma_/r);
  // (rIJ/sigma)^-6
  scalar ir6 = ir2*ir2*ir2;
  return 4.0 * epsilon_*(ir6*(ir6 - 1.0));
}
bool lennardJones::read(const dictionary& pairPotentialProperties)
{
  pairPotential::read(pairPotentialProperties);
  lennardJonesCoeffs_ = pairPotentialProperties.subDict(typeName + "Coeffs");
  lennardJonesCoeffs_.lookup("sigma") >> sigma_;
  lennardJonesCoeffs_.lookup("epsilon") >> epsilon_;
  return true;
}
}  // namespace pairPotentials
}  // namespace mousse
