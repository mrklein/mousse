// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "harmonic_spring.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace tetherPotentials {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(harmonicSpring, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  tetherPotential,
  harmonicSpring,
  dictionary
);


// Constructors 
harmonicSpring::harmonicSpring
(
  const word& name,
  const dictionary& tetherPotentialProperties
)
:
  tetherPotential{name, tetherPotentialProperties},
  harmonicSpringCoeffs_
  {
    tetherPotentialProperties.subDict(typeName + "Coeffs")
  },
  springConstant_{readScalar(harmonicSpringCoeffs_.lookup("springConstant"))}
{}


// Member Functions 
scalar harmonicSpring::energy(const vector r) const
{
  return 0.5*springConstant_*magSqr(r);
}


vector harmonicSpring::force(const vector r) const
{
  return -springConstant_*r;
}


bool harmonicSpring::read(const dictionary& tetherPotentialProperties)
{
  tetherPotential::read(tetherPotentialProperties);
  harmonicSpringCoeffs_ =
    tetherPotentialProperties.subDict(typeName + "Coeffs");
  harmonicSpringCoeffs_.lookup("springConstant") >> springConstant_;
  return true;
}

}  // namespace tetherPotentials
}  // namespace mousse

