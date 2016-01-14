// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "abort_calculation.hpp"
#include "dictionary.hpp"
#include "error.hpp"
#include "time.hpp"
#include "os_specific.hpp"
#include "pstream_reduce_ops.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(abortCalculation, 0);
}
// Static Data Members
namespace mousse
{
  template<>
  const char* mousse::NamedEnum
  <
    mousse::abortCalculation::actionType,
    3
  >::names[] =
  {
    "noWriteNow",
    "writeNow",
    "nextWrite"
  };
}
const mousse::NamedEnum<mousse::abortCalculation::actionType, 3>
  mousse::abortCalculation::actionTypeNames_;
// Private Member Functions 
void mousse::abortCalculation::removeFile() const
{
  bool hasAbort = isFile(abortFile_);
  reduce(hasAbort, orOp<bool>());
  if (hasAbort && Pstream::master())
  {
    // cleanup ABORT file (on master only)
    rm(abortFile_);
  }
}
// Constructors 
mousse::abortCalculation::abortCalculation
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  abortFile_{"$MOUSSE_CASE/" + name},
  action_{nextWrite}
{
  abortFile_.expand();
  read(dict);
  // remove any old files from previous runs
  removeFile();
}
// Destructor 
mousse::abortCalculation::~abortCalculation()
{}
// Member Functions 
void mousse::abortCalculation::read(const dictionary& dict)
{
  if (dict.found("action"))
  {
    action_ = actionTypeNames_.read(dict.lookup("action"));
  }
  else
  {
    action_ = nextWrite;
  }
  if (dict.readIfPresent("fileName", abortFile_))
  {
    abortFile_.expand();
  }
}
void mousse::abortCalculation::execute()
{
  bool hasAbort = isFile(abortFile_);
  reduce(hasAbort, orOp<bool>());
  if (hasAbort)
  {
    switch (action_)
    {
      case noWriteNow :
      {
        if (obr_.time().stopAt(Time::saNoWriteNow))
        {
          Info<< "USER REQUESTED ABORT (timeIndex="
            << obr_.time().timeIndex()
            << "): stop without writing data"
            << endl;
        }
        break;
      }
      case writeNow :
      {
        if (obr_.time().stopAt(Time::saWriteNow))
        {
          Info<< "USER REQUESTED ABORT (timeIndex="
            << obr_.time().timeIndex()
            << "): stop+write data"
            << endl;
        }
        break;
      }
      case nextWrite :
      {
        if (obr_.time().stopAt(Time::saNextWrite))
        {
          Info<< "USER REQUESTED ABORT (timeIndex="
            << obr_.time().timeIndex()
            << "): stop after next data write"
            << endl;
        }
        break;
      }
    }
  }
}
void mousse::abortCalculation::end()
{
  removeFile();
}
void mousse::abortCalculation::timeSet()
{
  // Do nothing - only valid on execute
}
void mousse::abortCalculation::write()
{
  // Do nothing - only valid on execute
}
