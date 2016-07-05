// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multi_motion.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace solidBodyMotionFunctions {

DEFINE_TYPE_NAME_AND_DEBUG(multiMotion, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  solidBodyMotionFunction,
  multiMotion,
  dictionary
);

}
}


// Constructors 
mousse::solidBodyMotionFunctions::multiMotion::multiMotion
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
mousse::solidBodyMotionFunctions::multiMotion::~multiMotion()
{}


// Member Functions 
mousse::septernion
mousse::solidBodyMotionFunctions::multiMotion::transformation() const
{
  scalar t = time_.value();
  septernion TR = SBMFs_[0].transformation();
  for (label i = 1; i < SBMFs_.size(); i++) {
    TR *= SBMFs_[i].transformation();
  }
  Info << "solidBodyMotionFunctions::multiMotion::transformation(): "
    << "Time = " << t << " transformation: " << TR << endl;
  return TR;
}


bool mousse::solidBodyMotionFunctions::multiMotion::read
(
  const dictionary& SBMFCoeffs
)
{
  solidBodyMotionFunction::read(SBMFCoeffs);
  label i = 0;
  SBMFs_.setSize(SBMFCoeffs_.size());
  FOR_ALL_CONST_ITER(IDLList<entry>, SBMFCoeffs_, iter) {
    if (iter().isDict()) {
      SBMFs_.set
      (
        i,
        solidBodyMotionFunction::New(iter().dict(), time_)
      );
      Info << "Constructed SBMF " << i << " : "
        << iter().keyword() << " of type "
        << SBMFs_[i].type() << endl;
      i++;
    }
  }
  SBMFs_.setSize(i);
  return true;
}

