#ifndef OS_SPECIFIC_POSIX_SIGNALS_SIG_INT_HPP_
#define OS_SPECIFIC_POSIX_SIGNALS_SIG_INT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sigInt
// Description
//   Signal handler for INT interupt.
//   The standard interupt handler is overridden to ensure that the
//   runningJob file is removed.
// See Also
//   mousse::JobInfo

#include <signal.h>


namespace mousse {

class sigInt
{
  // Private data
    //- Saved old signal trapping setting
    static struct sigaction oldAction_;
  // Private Member Functions
    static void sigHandler(int);
public:
  // Constructors
    //- Construct null
    sigInt();
  //- Destructor
  ~sigInt();
  // Member functions
    //- Activate SIGINT signal handler
    void set(const bool verbose);
};

}  // namespace mousse

#endif
