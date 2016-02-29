#ifndef OS_SPECIFIC_POSIX_CLOCK_TIME_CLOCK_TIME_HPP_
#define OS_SPECIFIC_POSIX_CLOCK_TIME_CLOCK_TIME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::clockTime
// Description
//   Starts timing (using rtc) and returns elapsed time from start.
//   Better resolution (2uSec instead of ~20mSec) than cpuTime.
// SourceFiles
//   clock_time.cpp
#include <sys/types.h>
#if defined(darwin64)
#include <sys/time.h>
#endif
namespace mousse
{
class clockTime
{
  // Private data
    //- Time structure used
    typedef struct timeval timeType;
    timeType startTime_;
    mutable timeType lastTime_;
    mutable timeType newTime_;
  // Private Member Functions
    //- Retrieve the current time values from the system
    static void getTime(timeType&);
    //- Difference between two times
    static double timeDifference(const timeType& beg, const timeType& end);
public:
  // Constructors
    //- Construct with the current clock time
    clockTime();
  // Member Functions
    //- Return time (in seconds) from the start
    double elapsedTime() const;
    //- Return time (in seconds) since last call to timeIncrement()
    double timeIncrement() const;
};
}  // namespace mousse
#endif
