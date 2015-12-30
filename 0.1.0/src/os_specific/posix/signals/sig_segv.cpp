// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_segv.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "iostreams.hpp"
// Static Data Members
struct sigaction mousse::sigSegv::oldAction_;
// Private Member Functions 
void mousse::sigSegv::sigHandler(int)
{
  // Reset old handling
  if (sigaction(SIGSEGV, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigSegv::sigHandler()"
    )   << "Cannot reset SIGSEGV trapping"
      << abort(FatalError);
  }
  // Update jobInfo file
  jobInfo.signalEnd();
  error::printStack(Perr);
  // Throw signal (to old handler)
  raise(SIGSEGV);
}
// Constructors 
mousse::sigSegv::sigSegv()
{
  oldAction_.sa_handler = NULL;
}
// Destructor 
mousse::sigSegv::~sigSegv()
{
  // Reset old handling
  if (sigaction(SIGSEGV, &oldAction_, NULL) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigSegv::~sigSegv()"
    )   << "Cannot reset SIGSEGV trapping"
      << abort(FatalError);
  }
}
// Member Functions 
void mousse::sigSegv::set(const bool)
{
  if (oldAction_.sa_handler)
  {
    FatalErrorIn
    (
      "mousse::sigSegv::set()"
    )   << "Cannot call sigSegv::set() more than once"
      << abort(FatalError);
  }
  struct sigaction newAction;
  newAction.sa_handler = sigHandler;
  newAction.sa_flags = SA_NODEFER;
  sigemptyset(&newAction.sa_mask);
  if (sigaction(SIGSEGV, &newAction, &oldAction_) < 0)
  {
    FatalErrorIn
    (
      "mousse::sigSegv::set()"
    )   << "Cannot set SIGSEGV trapping"
      << abort(FatalError);
  }
}
