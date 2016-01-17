// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "system_call.hpp"
#include "time.hpp"
#include "dynamic_code.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(systemCall, 0);
}
// Constructors 
mousse::systemCall::systemCall
(
  const word& name,
  const objectRegistry&,
  const dictionary& dict,
  const bool
)
:
  name_(name),
  executeCalls_(),
  endCalls_(),
  writeCalls_()
{
  read(dict);
}
// Destructor 
mousse::systemCall::~systemCall()
{}
// Member Functions 
void mousse::systemCall::read(const dictionary& dict)
{
  dict.readIfPresent("executeCalls", executeCalls_);
  dict.readIfPresent("endCalls", endCalls_);
  dict.readIfPresent("writeCalls", writeCalls_);
  if (executeCalls_.empty() && endCalls_.empty() && writeCalls_.empty())
  {
    WARNING_IN("mousse::system::read(const dictionary&)")
      << "no executeCalls, endCalls or writeCalls defined."
      << endl;
  }
  else if (!dynamicCode::allowSystemOperations)
  {
    FATAL_ERROR_IN("systemCall::read(const dictionary&)")
      << "Executing user-supplied system calls is not enabled by "
      << "default because of " << nl
      << "security issues.  If you trust the case you can enable this "
      << "facility by " << nl
      << "adding to the InfoSwitches setting in the system controlDict:"
      << nl << nl
      << "    allowSystemOperations 1" << nl << nl
      << "The system controlDict is either" << nl << nl
      << "    ~/.OpenFOAM/$WM_PROJECT_VERSION/controlDict" << nl << nl
      << "or" << nl << nl
      << "    $WM_PROJECT_DIR/etc/controlDict" << nl << nl
      << exit(FatalError);
  }
}
void mousse::systemCall::execute()
{
  FOR_ALL(executeCalls_, callI)
  {
    mousse::system(executeCalls_[callI]);
  }
}
void mousse::systemCall::end()
{
  FOR_ALL(endCalls_, callI)
  {
    mousse::system(endCalls_[callI]);
  }
}
void mousse::systemCall::timeSet()
{
  // Do nothing
}
void mousse::systemCall::write()
{
  FOR_ALL(writeCalls_, callI)
  {
    mousse::system(writeCalls_[callI]);
  }
}
