// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_stop_at_write_now.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "iostreams.hpp"
#include "time.hpp"


// Static Data Members

namespace mousse {

// Signal number to catch
int sigStopAtWriteNow::signal_
{
  debug::optimisationSwitch("stopAtWriteNowSignal", -1)
};


// Register re-reader
class addstopAtWriteNowSignalToOpt
:
  public ::mousse::simpleRegIOobject
{
public:
  addstopAtWriteNowSignalToOpt(const char* name)
  :
    ::mousse::simpleRegIOobject{mousse::debug::addOptimisationObject, name}
  {}
  virtual ~addstopAtWriteNowSignalToOpt()
  {}
  virtual void readData(mousse::Istream& is)
  {
    sigStopAtWriteNow::signal_ = readLabel(is);
    sigStopAtWriteNow::set(true);
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os << sigStopAtWriteNow::signal_;
  }
};


addstopAtWriteNowSignalToOpt addstopAtWriteNowSignalToOpt_
{
  "stopAtWriteNowSignal"
};


}

static mousse::Time const* runTimePtr_ = NULL;
struct sigaction mousse::sigStopAtWriteNow::oldAction_;

// Private Member Functions 
void mousse::sigStopAtWriteNow::sigHandler(int)
{
  // Reset old handling
  if (sigaction(signal_, &oldAction_, NULL) < 0) {
    FATAL_ERROR_IN
    (
      "mousse::sigStopAtWriteNow::sigHandler(int)"
    )
    << "Cannot reset " << signal_ << " trapping"
    << abort(FatalError);
  }
  // Update jobInfo file
  jobInfo.signalEnd();
  Info << "sigStopAtWriteNow :"
    << " setting up write and stop at end of the next iteration"
    << nl << endl;
  runTimePtr_->stopAt(Time::saWriteNow);
}


// Constructors 
mousse::sigStopAtWriteNow::sigStopAtWriteNow()
{}


mousse::sigStopAtWriteNow::sigStopAtWriteNow
(
  const bool verbose,
  const Time& runTime
)
{
  // Store runTime
  runTimePtr_ = &runTime;
  set(verbose);
}


// Destructor 
mousse::sigStopAtWriteNow::~sigStopAtWriteNow()
{
  // Reset old handling
  if (signal_ > 0) {
    if (sigaction(signal_, &oldAction_, NULL) < 0) {
      FATAL_ERROR_IN
      (
        "mousse::sigStopAtWriteNow::~sigStopAtWriteNow()"
      )
      << "Cannot reset " << signal_ << " trapping"
      << abort(FatalError);
    }
  }
}


// Member Functions 
void mousse::sigStopAtWriteNow::set(const bool verbose)
{
  if (signal_ < 0)
    return;
  // Check that the signal is different from the writeNowSignal
  if (sigWriteNow::signal_ == signal_) {
    FATAL_ERROR_IN
    (
      "mousse::sigStopAtWriteNow::sigStopAtWriteNow"
      "(const bool, const Time&)"
    )
    << "stopAtWriteNowSignal : " << signal_
    << " cannot be the same as the writeNowSignal."
    << " Please change this in the controlDict ("
    << findEtcFile("controlDict", false) << ")."
    << exit(FatalError);
  }
  struct sigaction newAction;
  newAction.sa_handler = sigHandler;
  newAction.sa_flags = SA_NODEFER;
  sigemptyset(&newAction.sa_mask);
  if (sigaction(signal_, &newAction, &oldAction_) < 0) {
    FATAL_ERROR_IN
    (
      "mousse::sigStopAtWriteNow::sigStopAtWriteNow"
      "(const bool, const Time&)"
    )
    << "Cannot set " << signal_ << " trapping"
    << abort(FatalError);
  }
  if (verbose) {
    Info << "sigStopAtWriteNow :"
      << " Enabling writing and stopping upon signal " << signal_
      << endl;
  }
}


bool mousse::sigStopAtWriteNow::active() const
{
  return signal_ > 0;
}

