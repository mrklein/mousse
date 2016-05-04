// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "restrained_harmonic_spring.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace tetherPotentials {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(restrainedHarmonicSpring, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  tetherPotential,
  restrainedHarmonicSpring,
  dictionary
);


// Constructors 
restrainedHarmonicSpring::restrainedHarmonicSpring
(
  const word& name,
  const dictionary& tetherPotentialProperties
)
:
  tetherPotential{name, tetherPotentialProperties},
  restrainedHarmonicSpringCoeffs_
  {
    tetherPotentialProperties.subDict(typeName + "Coeffs")
  },
  springConstant_
  {
    readScalar(restrainedHarmonicSpringCoeffs_.lookup("springConstant"))
  },
  rR_
  {
    readScalar(restrainedHarmonicSpringCoeffs_.lookup("rR"))
  }
{}


// Member Functions 
scalar restrainedHarmonicSpring::energy(const vector r) const
{
  scalar magR = mag(r);
  if (magR < rR_) {
    return 0.5*springConstant_*magSqr(r);
  } else {
    return 0.5*springConstant_*rR_*rR_
     + springConstant_*rR_*(magR - rR_);
  }
}


vector restrainedHarmonicSpring::force(const vector r) const
{
  scalar magR = mag(r);
  if (magR < rR_) {
    return -springConstant_ * r;
  } else {
    return -springConstant_ * rR_ * r / magR;
  }
}


bool restrainedHarmonicSpring::read
(
  const dictionary& tetherPotentialProperties
)
{
  tetherPotential::read(tetherPotentialProperties);
  restrainedHarmonicSpringCoeffs_ =
    tetherPotentialProperties.subDict(typeName + "Coeffs");
  restrainedHarmonicSpringCoeffs_.lookup("springConstant") >> springConstant_;
  restrainedHarmonicSpringCoeffs_.lookup("rR") >> rR_;
  return true;
}

}  // namespace tetherPotentials
}  // namespace mousse

