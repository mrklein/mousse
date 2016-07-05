// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tabulated_6dof_acceleration.hpp"
#include "tuple2.hpp"
#include "ifstream.hpp"
#include "interpolate_spline_xy.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(tabulated6DoFAcceleration, 0);

}


// Constructors 
mousse::tabulated6DoFAcceleration::tabulated6DoFAcceleration
(
  const dictionary& accelerationCoeffs,
  const Time& runTime
)
:
  time_{runTime},
  accelerationCoeffs_{accelerationCoeffs}
{
  read(accelerationCoeffs);
}


// Destructor 
mousse::tabulated6DoFAcceleration::~tabulated6DoFAcceleration()
{}


// Member Functions 
mousse::tabulated6DoFAcceleration::accelerationVectors
mousse::tabulated6DoFAcceleration::acceleration() const
{
  scalar t = time_.value();
  if (t < times_[0]) {
    FATAL_ERROR_IN
    (
      "tabulated6DoFAcceleration::acceleration()"
    )
    << "current time (" << t
    << ") is less than the minimum in the data table ("
    << times_[0] << ')'
    << exit(FatalError);
  }
  if (t > times_.last()) {
    FATAL_ERROR_IN
    (
      "tabulated6DoFAcceleration::acceleration()"
    )
    << "current time (" << t
    << ") is greater than the maximum in the data table ("
    << times_.last() << ')'
    << exit(FatalError);
  }
  accelerationVectors avs =
    interpolateSplineXY
    (
      t,
      times_,
      values_
    );
  Info << "tabulated6DoFAcceleration::acceleration(): "
    << "Time = " << t << " accelerations: " << avs << endl;
  return avs;
}


bool mousse::tabulated6DoFAcceleration::read
(
  const dictionary& accelerationCoeffs
)
{
  accelerationCoeffs_ = accelerationCoeffs;
  // If the timeDataFileName has changed read the file
  fileName newTimeDataFileName
  {
    fileName{accelerationCoeffs_.lookup("timeDataFileName")}.expand()
  };
  if (newTimeDataFileName != timeDataFileName_) {
    timeDataFileName_ = newTimeDataFileName;
    IFstream dataStream{timeDataFileName_};
    if (dataStream.good()) {
      List<Tuple2<scalar, accelerationVectors>> timeValues{dataStream};
      times_.setSize(timeValues.size());
      values_.setSize(timeValues.size());
      FOR_ALL(timeValues, i) {
        times_[i] = timeValues[i].first();
        values_[i] = timeValues[i].second();
      }
    } else {
      FATAL_ERROR_IN
      (
        "tabulated6DoFAcceleration::read(const dictionary&)"
      )
      << "Cannot open time data file " << timeDataFileName_
      << exit(FatalError);
    }
  }
  return true;
}

