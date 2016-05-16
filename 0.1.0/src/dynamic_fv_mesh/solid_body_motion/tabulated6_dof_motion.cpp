// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated6_dof_motion.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tuple2.hpp"
#include "ifstream.hpp"
#include "interpolate_spline_xy.hpp"
#include "mathematical_constants.hpp"


using namespace mousse::constant::mathematical;

// Static Data Members
namespace mousse {
namespace solidBodyMotionFunctions {

DEFINE_TYPE_NAME_AND_DEBUG(tabulated6DoFMotion, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  solidBodyMotionFunction,
  tabulated6DoFMotion,
  dictionary
);

}
}


// Constructors 
mousse::solidBodyMotionFunctions::tabulated6DoFMotion::tabulated6DoFMotion
(
  const dictionary& SBMFCoeffs,
  const Time& runTime
)
:
  solidBodyMotionFunction{SBMFCoeffs, runTime}
{
  read(SBMFCoeffs);
}


// Destructor 
mousse::solidBodyMotionFunctions::tabulated6DoFMotion::~tabulated6DoFMotion()
{}


// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::tabulated6DoFMotion::transformation() const
{
  scalar t = time_.value();
  if (t < times_[0]) {
    FATAL_ERROR_IN
    (
      "solidBodyMotionFunctions::tabulated6DoFMotion::transformation()"
    )
    << "current time (" << t
    << ") is less than the minimum in the data table ("
    << times_[0] << ')'
    << exit(FatalError);
  }
  if (t > times_.last()) {
    FATAL_ERROR_IN
    (
      "solidBodyMotionFunctions::tabulated6DoFMotion::transformation()"
    )
    << "current time (" << t
    << ") is greater than the maximum in the data table ("
    << times_.last() << ')'
    << exit(FatalError);
  }
  translationRotationVectors TRV =
    interpolateSplineXY
    (
      t,
      times_,
      values_
    );
  // Convert the rotational motion from deg to rad
  TRV[1] *= pi/180.0;
  quaternion R{TRV[1].x(), TRV[1].y(), TRV[1].z()};
  septernion TR{septernion(CofG_ + TRV[0])*R*septernion(-CofG_)};
  Info << "solidBodyMotionFunctions::tabulated6DoFMotion::transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}


bool mousse::solidBodyMotionFunctions::tabulated6DoFMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  // If the timeDataFileName has changed read the file
  fileName newTimeDataFileName
  {
    fileName{SBMFCoeffs_.lookup("timeDataFileName")}.expand()
  };
  if (newTimeDataFileName != timeDataFileName_) {
    timeDataFileName_ = newTimeDataFileName;
    IFstream dataStream{timeDataFileName_};
    if (dataStream.good()) {
      List<Tuple2<scalar, translationRotationVectors>> timeValues{dataStream};
      times_.setSize(timeValues.size());
      values_.setSize(timeValues.size());
      FOR_ALL(timeValues, i) {
        times_[i] = timeValues[i].first();
        values_[i] = timeValues[i].second();
      }
    } else {
      FATAL_ERROR_IN
      (
        "solidBodyMotionFunctions::tabulated6DoFMotion::read"
        "(const dictionary&)"
      )
      << "Cannot open time data file " << timeDataFileName_
      << exit(FatalError);
    }
  }
  SBMFCoeffs_.lookup("CofG") >> CofG_;
  return true;
}

