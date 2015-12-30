// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_quit.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "iostreams.hpp"
// Static Data Members
struct sigaction mousse::sigQuit::oldAction_;
// Private Member Functions 
void mousse::sigQuit::sigHandler(int)
{
  // Reset old handling
  if (sigaction(SIGQUIT, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigQuit::sigHandler()"
    )   << "Cannot reset SIGQUIT trapping"
      << abort(FatalError);
  }
  // Update jobInfo file
  jobInfo.signalEnd();
  error::printStack(Perr);
  // Throw signal (to old handler)
  raise(SIGQUIT);
}
// Constructors 
mousse::sigQuit::sigQuit()
{
  oldAction_.sa_handler = NULL;
}
// Destructor 
mousse::sigQuit::~sigQuit()
{
  // Reset old handling
  if (oldAction_.sa_handler && sigaction(SIGQUIT, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigQuit::~sigQuit()"
    )   << "Cannot reset SIGQUIT trapping"
      << abort(FatalError);
  }
}
// Member Functions 
void mousse::sigQuit::set(const bool verbose)
{
  if (oldAction_.sa_handler)
  {
    FatalErrorIn
    (
      "mousse::sigQuit::set()"
    )   << "Cannot call sigQuit::set() more than once"
      << abort(FatalError);
  }
  struct sigaction newAction;
  newAction.sa_handler = sigHandler;
  newAction.sa_flags = SA_NODEFER;
  sigemptyset(&newAction.sa_mask);
  if (sigaction(SIGQUIT, &newAction, &oldAction_) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigQuit::set()"
    )   << "Cannot set SIGQUIT trapping"
      << abort(FatalError);
  }
}
