// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "exponential_repulsion.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace pairPotentials
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(exponentialRepulsion, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  pairPotential,
  exponentialRepulsion,
  dictionary
);
// Constructors 
exponentialRepulsion::exponentialRepulsion
(
  const word& name,
  const dictionary& exponentialRepulsion
)
:
  pairPotential(name, exponentialRepulsion),
  exponentialRepulsionCoeffs_
  (
    exponentialRepulsion.subDict(typeName + "Coeffs")
  ),
  rm_(readScalar(exponentialRepulsionCoeffs_.lookup("rm"))),
  epsilon_(readScalar(exponentialRepulsionCoeffs_.lookup("epsilon")))
{
  setLookupTables();
}
// Member Functions 
scalar exponentialRepulsion::unscaledEnergy(const scalar r) const
{
  return epsilon_ * exp(-r/rm_);
}
bool exponentialRepulsion::read(const dictionary& exponentialRepulsion)
{
  pairPotential::read(exponentialRepulsion);
  exponentialRepulsionCoeffs_ =
    exponentialRepulsion.subDict(typeName + "Coeffs");
  exponentialRepulsionCoeffs_.lookup("rm") >> rm_;
  exponentialRepulsionCoeffs_.lookup("epsilon") >> epsilon_;
  return true;
}
}  // namespace pairPotentials
}  // namespace mousse
