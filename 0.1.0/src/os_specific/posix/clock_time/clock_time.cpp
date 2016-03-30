// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "clock_time.hpp"
#include <sys/time.h>


// Private Member Functions 
void mousse::clockTime::getTime(timeType& t)
{
  gettimeofday(&t, 0);
}


double mousse::clockTime::timeDifference(const timeType& beg, const timeType& end)
{
  return end.tv_sec - beg.tv_sec + 1e-6*(end.tv_usec - beg.tv_usec);
}


// Constructors 
mousse::clockTime::clockTime()
{
  getTime(startTime_);
  lastTime_ = startTime_;
  newTime_ = startTime_;
}


// Member Functions 
double mousse::clockTime::elapsedTime() const
{
  getTime(newTime_);
  return timeDifference(startTime_, newTime_);
}


double mousse::clockTime::timeIncrement() const
{
  lastTime_ = newTime_;
  getTime(newTime_);
  return timeDifference(lastTime_, newTime_);
}

