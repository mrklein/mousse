// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "oscillating_linear_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace solidBodyMotionFunctions
{
  DEFINE_TYPE_NAME_AND_DEBUG(oscillatingLinearMotion, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    solidBodyMotionFunction,
    oscillatingLinearMotion,
    dictionary
  );
}
}
// Constructors 
mousse::solidBodyMotionFunctions::oscillatingLinearMotion::oscillatingLinearMotion
(
  const dictionary& SBMFCoeffs,
  const Time& runTime
)
:
  solidBodyMotionFunction(SBMFCoeffs, runTime)
{
  read(SBMFCoeffs);
}
// Destructor 
mousse::solidBodyMotionFunctions::oscillatingLinearMotion::
~oscillatingLinearMotion()
{}
// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::oscillatingLinearMotion::transformation() const
{
  scalar t = time_.value();
  const vector displacement = amplitude_*sin(omega_*t);
  quaternion R(0, 0, 0);
  septernion TR(septernion(displacement)*R);
  Info<< "solidBodyMotionFunctions::oscillatingLinearMotion::"
    << "transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}
bool mousse::solidBodyMotionFunctions::oscillatingLinearMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  SBMFCoeffs_.lookup("amplitude") >> amplitude_;
  SBMFCoeffs_.lookup("omega") >> omega_;
  return true;
}
