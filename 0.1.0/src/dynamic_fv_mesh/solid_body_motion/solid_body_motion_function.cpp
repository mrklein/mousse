// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_body_motion_function.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(solidBodyMotionFunction, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(solidBodyMotionFunction, dictionary);
}
// Constructors 
mousse::solidBodyMotionFunction::solidBodyMotionFunction
(
  const dictionary& SBMFCoeffs,
  const Time& runTime
)
:
  SBMFCoeffs_
  (
    SBMFCoeffs.subDict
    (
      word(SBMFCoeffs.lookup("solidBodyMotionFunction")) + "Coeffs"
    )
  ),
  time_(runTime)
{}
// Destructor 
mousse::solidBodyMotionFunction::~solidBodyMotionFunction()
{}
// Member Functions 
bool mousse::solidBodyMotionFunction::read(const dictionary& SBMFCoeffs)
{
  SBMFCoeffs_ = SBMFCoeffs.subDict(type() + "Coeffs");
  return true;
}
void mousse::solidBodyMotionFunction::writeData(Ostream& os) const
{
  os << SBMFCoeffs_;
}
