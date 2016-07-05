// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sig_write_now.hpp"
#include "error.hpp"
#include "job_info.hpp"
#include "iostreams.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

// Signal number to catch
int sigWriteNow::signal_
{
  debug::optimisationSwitch("writeNowSignal", -1)
};

// Register re-reader
class addwriteNowSignalToOpt
:
  public ::mousse::simpleRegIOobject
{
public:
  addwriteNowSignalToOpt(const char* name)
  :
    ::mousse::simpleRegIOobject{mousse::debug::addOptimisationObject, name}
  {}
  virtual ~addwriteNowSignalToOpt()
  {}
  virtual void readData(mousse::Istream& is)
  {
    sigWriteNow::signal_ = readLabel(is);
    sigWriteNow::set(true);
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os << sigWriteNow::signal_;
  }
};

addwriteNowSignalToOpt addwriteNowSignalToOpt_{"writeNowSignal"};

}


static mousse::Time* runTimePtr_ = NULL;
struct sigaction mousse::sigWriteNow::oldAction_;


// Private Member Functions 
void mousse::sigWriteNow::sigHandler(int)
{
  Info << "sigWriteNow :"
    << " setting up write at end of the next iteration" << nl << endl;
  runTimePtr_->writeOnce();
}


// Constructors 
mousse::sigWriteNow::sigWriteNow()
{}


mousse::sigWriteNow::sigWriteNow(const bool verbose, Time& runTime)
{
  // Store runTime
  runTimePtr_ = &runTime;
  set(verbose);
}


// Destructor 
mousse::sigWriteNow::~sigWriteNow()
{
  // Reset old handling
  if (signal_ > 0) {
    if (sigaction(signal_, &oldAction_, NULL) < 0) {
      FATAL_ERROR_IN
      (
        "mousse::sigWriteNow::~sigWriteNow()"
      )
      << "Cannot reset " << signal_ << " trapping"
      << abort(FatalError);
    }
  }
}


// Member Functions 
void mousse::sigWriteNow::set(const bool verbose)
{
  if (signal_ < 0)
    return;

  struct sigaction newAction;
  newAction.sa_handler = sigHandler;
  newAction.sa_flags = SA_NODEFER;
  sigemptyset(&newAction.sa_mask);
  if (sigaction(signal_, &newAction, &oldAction_) < 0) {
    FATAL_ERROR_IN
    (
      "mousse::sigWriteNow::sigWriteNow(const bool, const Time&)"
    )
    << "Cannot set " << signal_ << " trapping" << abort(FatalError);
  }
  if (verbose) {
    Info << "sigWriteNow :" << " Enabling writing upon signal " << signal_
      << endl;
  }
}


bool mousse::sigWriteNow::active() const
{
  return signal_ > 0;
}

