// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "oscillating_rotating_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mathematical_constants.hpp"
using namespace mousse::constant::mathematical;
// Static Data Members
namespace mousse
{
namespace solidBodyMotionFunctions
{
  DEFINE_TYPE_NAME_AND_DEBUG(oscillatingRotatingMotion, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    solidBodyMotionFunction,
    oscillatingRotatingMotion,
    dictionary
  );
}
}
// Constructors 
mousse::solidBodyMotionFunctions::oscillatingRotatingMotion::
oscillatingRotatingMotion
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
mousse::solidBodyMotionFunctions::oscillatingRotatingMotion::
~oscillatingRotatingMotion()
{}
// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::oscillatingRotatingMotion::
transformation() const
{
  scalar t = time_.value();
  vector eulerAngles = amplitude_*sin(omega_*t);
  // Convert the rotational motion from deg to rad
  eulerAngles *= pi/180.0;
  quaternion R(eulerAngles.x(), eulerAngles.y(), eulerAngles.z());
  septernion TR(septernion(origin_)*R*septernion(-origin_));
  Info<< "solidBodyMotionFunctions::oscillatingRotatingMotion::"
    << "transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}
bool mousse::solidBodyMotionFunctions::oscillatingRotatingMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  SBMFCoeffs_.lookup("origin") >> origin_;
  SBMFCoeffs_.lookup("amplitude") >> amplitude_;
  SBMFCoeffs_.lookup("omega") >> omega_;
  return true;
}
