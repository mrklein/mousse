// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cpu_time.hpp"
#include <unistd.h>


// Static Members 
const long mousse::cpuTime::Hz_(sysconf(_SC_CLK_TCK));

// Private Member Functions 
void mousse::cpuTime::getTime(timeType& t)
{
  times(&t);
}


double mousse::cpuTime::timeDifference(const timeType& beg, const timeType& end)
{
  return
  (
    double
    (
      (end.tms_utime + end.tms_stime) - (beg.tms_utime + beg.tms_stime)
    )/Hz_
  );
}


// Constructors 
mousse::cpuTime::cpuTime()
{
  getTime(startTime_);
  lastTime_ = startTime_;
  newTime_ = startTime_;
}


// Member Functions 
double mousse::cpuTime::elapsedCpuTime() const
{
  getTime(newTime_);
  return timeDifference(startTime_, newTime_);
}


double mousse::cpuTime::cpuTimeIncrement() const
{
  lastTime_ = newTime_;
  getTime(newTime_);
  return timeDifference(lastTime_, newTime_);
}

