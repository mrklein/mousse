// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include <unistd.h>
#include "error.hpp"
#include "timer.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(timer, 0);
jmp_buf timer::envAlarm;
struct sigaction timer::oldAction_;
unsigned int timer::oldTimeOut_ = 0;
}
// Static Member Functions
void mousse::timer::signalHandler(int)
{
  if (debug)
  {
    Info<< "mousse::timer::signalHandler(int sig) : "
      << " timed out. Jumping."
      << endl;
  }
  longjmp(envAlarm, 1);
}
// Constructors 
mousse::timer::timer(const unsigned int newTimeOut)
:
  newTimeOut_(newTimeOut)
{
  if (newTimeOut > 0)
  {
    // Is singleton since handler is static function
    if (oldTimeOut_ != 0)
    {
      FatalErrorIn
      (
        "mousse::timer::timer(const unsigned int)"
      )   << "timer already used."
        << abort(FatalError);
    }
    // Install alarm signal handler:
    // - do not block any signals while in it
    // - clear list of signals to mask
    struct sigaction newAction;
    newAction.sa_handler = timer::signalHandler;
    newAction.sa_flags = SA_NODEFER;
    sigemptyset(&newAction.sa_mask);
    if (sigaction(SIGALRM, &newAction, &oldAction_) < 0)
    {
      FatalErrorIn
      (
        "mousse::timer::timer(const unsigned int)"
      )   << "sigaction(SIGALRM) error"
        << abort(FatalError);
    }
    oldTimeOut_ = ::alarm(newTimeOut);
    if (debug)
    {
      Info<< "mousse::timer::timer(const unsigned int) : "
        << " installing timeout " << int(newTimeOut_)
        << " seconds"
        << " (overriding old timeout " << int(oldTimeOut_)
        << ")." << endl;
    }
  }
}
// Destructor 
mousse::timer::~timer()
{
  if (newTimeOut_ > 0)
  {
    if (debug)
    {
      Info<< "mousse::timer::~timer(const unsigned int) : timeOut="
        << int(newTimeOut_)
        << " : resetting timeOut to " << int(oldTimeOut_) << endl;
    }
    // Reset timer
    ::alarm(oldTimeOut_);
    oldTimeOut_ = 0;
    // Restore signal handler
    if (sigaction(SIGALRM, &oldAction_, NULL) < 0)
    {
      FatalErrorIn
      (
        "mousse::timer::~timer(const struct sigaction&"
        "const struct sigaction&)"
      )   << "sigaction(SIGALRM) error"
        << abort(FatalError);
    }
  }
}
