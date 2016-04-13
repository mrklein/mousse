#ifndef OS_SPECIFIC_POSIX_SIGNALS_SIG_STOP_AT_WRITE_NOW_HPP_
#define OS_SPECIFIC_POSIX_SIGNALS_SIG_STOP_AT_WRITE_NOW_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sigStopAtWriteNow
// Description
//   Signal handler for interupt defined by
//     OptimisationSwitches::stopAtWriteNowSignal
//   Write and stop the job.

#include <signal.h>


namespace mousse {

class Time;


class sigStopAtWriteNow
{
  // Private data
    //- Number of signal to use
    static int signal_;
    //- Saved old signal trapping setting
    static struct sigaction oldAction_;
  // Private Member Functions
    static void sigHandler(int);
public:
  //- wip. Have setter have access to signal_
  friend class addstopAtWriteNowSignalToOpt;
  // Constructors
    //- Construct null
    sigStopAtWriteNow();
    //- Construct from components
    sigStopAtWriteNow(const bool verbose, const Time& runTime);
  //- Destructor
  ~sigStopAtWriteNow();
  // Member functions
    //- (re)set signal catcher
    static void set(const bool verbose);
    //- Is active?
    bool active() const;
};

}  // namespace mousse

#endif
