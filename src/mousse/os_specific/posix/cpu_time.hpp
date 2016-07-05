#ifndef OS_SPECIFIC_POSIX_CPU_TIME_CPU_TIME_HPP_
#define OS_SPECIFIC_POSIX_CPU_TIME_CPU_TIME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cpuTime
// Description
//   Starts timing CPU usage and return elapsed time from start.
// SeeAlso
//   clockTime

#include <time.h>
#include <sys/times.h>


namespace mousse {

class cpuTime
{
  // Private data
    //- Time structure used
    typedef struct tms timeType;
    //- Clock-ticks per second
    static const long Hz_;
    //- The start time
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
    cpuTime();
  // Member Functions
    //- Return CPU time (in seconds) from the start
    double elapsedCpuTime() const;
    //- Return CPU time (in seconds) since last call to cpuTimeIncrement()
    double cpuTimeIncrement() const;
};

}  // namespace mousse

#endif
