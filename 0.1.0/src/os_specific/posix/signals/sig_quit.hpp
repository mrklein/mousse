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
// SourceFiles
//   sig_quit.cpp
#ifndef sig_quit_hpp_
#define sig_quit_hpp_
#include <signal.h>
namespace mousse
{
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
