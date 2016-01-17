// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rotating_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mathematical_constants.hpp"
using namespace mousse::constant::mathematical;
// Static Data Members
namespace mousse
{
namespace solidBodyMotionFunctions
{
  DEFINE_TYPE_NAME_AND_DEBUG(rotatingMotion, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    solidBodyMotionFunction,
    rotatingMotion,
    dictionary
  );
}
}
// Constructors 
mousse::solidBodyMotionFunctions::rotatingMotion::rotatingMotion
(
  const dictionary& SBMFCoeffs,
  const Time& runTime
)
:
  solidBodyMotionFunction(SBMFCoeffs, runTime),
  origin_(SBMFCoeffs_.lookup("origin")),
  axis_(SBMFCoeffs_.lookup("axis")),
  omega_(DataEntry<scalar>::New("omega", SBMFCoeffs_))
{}
// Destructor 
mousse::solidBodyMotionFunctions::rotatingMotion::~rotatingMotion()
{}
// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::rotatingMotion::transformation() const
{
  scalar t = time_.value();
  // Rotation around axis
  scalar angle = omega_->integrate(0, t);
  quaternion R(axis_, angle);
  septernion TR(septernion(origin_)*R*septernion(-origin_));
  Info<< "solidBodyMotionFunctions::rotatingMotion::transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}
bool mousse::solidBodyMotionFunctions::rotatingMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  omega_.reset
  (
    DataEntry<scalar>::New("omega", SBMFCoeffs_).ptr()
  );
  return true;
}
