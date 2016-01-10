// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "axis_rotation_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "unit_conversion.hpp"
using namespace mousse::constant::mathematical;
// Static Data Members
namespace mousse
{
namespace solidBodyMotionFunctions
{
  DEFINE_TYPE_NAME_AND_DEBUG(axisRotationMotion, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    solidBodyMotionFunction,
    axisRotationMotion,
    dictionary
  );
}
}
// Constructors 
mousse::solidBodyMotionFunctions::axisRotationMotion::axisRotationMotion
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
mousse::solidBodyMotionFunctions::axisRotationMotion::~axisRotationMotion()
{}
// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::axisRotationMotion::transformation() const
{
  scalar t = time_.value();
  // Rotation origin (in radians)
  vector omega
  (
    t*degToRad(radialVelocity_.x()),
    t*degToRad(radialVelocity_.y()),
    t*degToRad(radialVelocity_.z())
  );
  scalar magOmega = mag(omega);
  quaternion R(omega/magOmega, magOmega);
  septernion TR(septernion(origin_)*R*septernion(-origin_));
  Info<< "solidBodyMotionFunctions::axisRotationMotion::transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}
bool mousse::solidBodyMotionFunctions::axisRotationMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  SBMFCoeffs_.lookup("origin") >> origin_;
  SBMFCoeffs_.lookup("radialVelocity") >> radialVelocity_;
  return true;
}
