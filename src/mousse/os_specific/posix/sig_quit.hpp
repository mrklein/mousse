#ifndef OS_SPECIFIC_POSIX_SIGNALS_SIG_QUIT_HPP_
#define OS_SPECIFIC_POSIX_SIGNALS_SIG_QUIT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sigQuit
// Description
//   Signal handler for QUIT interupt.
//   The standard interupt handler is overridden to ensure that the
//   runningJob file is removed.
// See Also
//   mousse::JobInfo

#include <signal.h>


namespace mousse {

class sigQuit
{
  // Private data
    //- Saved old signal trapping setting
    static struct sigaction oldAction_;
  // Private Member Functions
    //- Handler for caught signals
    static void sigHandler(int);
public:
  // Constructors
    //- Construct null
    sigQuit();
  //- Destructor
  ~sigQuit();
  // Member functions
    //- Activate SIGQUIT signal handler
    void set(const bool verbose);
};

}  // namespace mousse

#endif
