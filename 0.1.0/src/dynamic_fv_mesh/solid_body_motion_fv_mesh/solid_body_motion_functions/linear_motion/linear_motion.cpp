// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace solidBodyMotionFunctions
{
  DEFINE_TYPE_NAME_AND_DEBUG(linearMotion, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    solidBodyMotionFunction,
    linearMotion,
    dictionary
  );
}
}
// Constructors 
mousse::solidBodyMotionFunctions::linearMotion::linearMotion
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
mousse::solidBodyMotionFunctions::linearMotion::~linearMotion()
{}
// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::linearMotion::transformation() const
{
  scalar t = time_.value();
  // Translation of centre of gravity with constant velocity
  const vector displacement = velocity_*t;
  quaternion R(0, 0, 0);
  septernion TR(septernion(displacement)*R);
  Info<< "solidBodyMotionFunctions::linearMotion::transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}
bool mousse::solidBodyMotionFunctions::linearMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  SBMFCoeffs_.lookup("velocity") >> velocity_;
  return true;
}
