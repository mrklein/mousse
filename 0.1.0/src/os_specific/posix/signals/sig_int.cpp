// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_int.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "iostreams.hpp"
// Static Data Members
struct sigaction mousse::sigInt::oldAction_;
// Private Member Functions 
void mousse::sigInt::sigHandler(int)
{
  // Reset old handling
  if (sigaction(SIGINT, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigInt::sigHandler()"
    )   << "Cannot reset SIGINT trapping"
      << abort(FatalError);
  }
  // Update jobInfo file
  jobInfo.signalEnd();
  // Throw signal (to old handler)
  raise(SIGINT);
}
// Constructors 
mousse::sigInt::sigInt()
{
  oldAction_.sa_handler = NULL;
}
// Destructor 
mousse::sigInt::~sigInt()
{
  // Reset old handling
  if (sigaction(SIGINT, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigInt::~sigInt()"
    )   << "Cannot reset SIGINT trapping"
      << abort(FatalError);
  }
}
// Member Functions 
void mousse::sigInt::set(const bool)
{
  if (oldAction_.sa_handler)
  {
    FatalErrorIn
    (
      "mousse::sigInt::set()"
    )   << "Cannot call sigInt::set() more than once"
      << abort(FatalError);
  }
  struct sigaction newAction;
  newAction.sa_handler = sigHandler;
  newAction.sa_flags = SA_NODEFER;
  sigemptyset(&newAction.sa_mask);
  if (sigaction(SIGINT, &newAction, &oldAction_) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigInt::set()"
    )   << "Cannot set SIGINT trapping"
      << abort(FatalError);
  }
}
