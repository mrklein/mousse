#ifndef OS_SPECIFIC_POSIX_SIGNALS_SIG_WRITE_NOW_HPP_
#define OS_SPECIFIC_POSIX_SIGNALS_SIG_WRITE_NOW_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sigWriteNow
// Description
//   Signal handler for interupt defined by OptimisationSwitches::writeNowSignal
//   Write once and continue.

#include <signal.h>


namespace mousse {

class Time;


class sigWriteNow
{
  // Private data
    //- Number of signal to use
    static int signal_;
    //- Saved old signal trapping setting
    static struct sigaction oldAction_;
  // Private Member Functions
    static void sigHandler(int);
public:
  friend class sigStopAtWriteNow;
  //- wip. Have setter have access to signal_
  friend class addwriteNowSignalToOpt;
  // Constructors
    //- Construct null
    sigWriteNow();
    //- Construct from components
    sigWriteNow(const bool verbose, Time& runTime);
  //- Destructor
  ~sigWriteNow();
  // Member functions
    //- (re)set signal catcher
    static void set(const bool verbose);
    //- Is active?
    bool active() const;
};

}  // namespace mousse

#endif
