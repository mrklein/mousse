// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_activated_file_update.hpp"
#include "object_registry.hpp"
#include "time.hpp"
#include "dictionary.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(timeActivatedFileUpdate, 0);

}


// Private Member Functions 
void mousse::timeActivatedFileUpdate::updateFile()
{
  label i = lastIndex_;
  while (i < timeVsFile_.size() - 1
         && timeVsFile_[i+1].first() < obr_.time().value()) {
    i++;
  }
  if (i > lastIndex_) {
    Info << nl << type() << ": copying file" << nl << timeVsFile_[i].second()
      << nl << "to:" << nl << fileToUpdate_ << nl << endl;
    cp(timeVsFile_[i].second(), fileToUpdate_);
    lastIndex_ = i;
  }
}


// Constructors 
mousse::timeActivatedFileUpdate::timeActivatedFileUpdate
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  active_{true},
  fileToUpdate_{dict.lookup("fileToUpdate")},
  timeVsFile_{},
  lastIndex_{-1}
{
  read(dict);
}


// Destructor 
mousse::timeActivatedFileUpdate::~timeActivatedFileUpdate()
{}


// Member Functions 
void mousse::timeActivatedFileUpdate::read(const dictionary& dict)
{
  if (!active_)
    return;
  dict.lookup("fileToUpdate") >> fileToUpdate_;
  dict.lookup("timeVsFile") >> timeVsFile_;
  lastIndex_ = -1;
  fileToUpdate_.expand();
  Info << type() << ": time vs file list:" << nl;
  FOR_ALL(timeVsFile_, i) {
    timeVsFile_[i].second() = timeVsFile_[i].second().expand();
    if (!isFile(timeVsFile_[i].second())) {
      FATAL_ERROR_IN("timeActivatedFileUpdate::read(const dictionary&)")
        << "File: " << timeVsFile_[i].second() << " not found"
        << nl << exit(FatalError);
    }
    Info << "    " << timeVsFile_[i].first() << tab
      << timeVsFile_[i].second() << endl;
  }
  Info << endl;
  updateFile();
}


void mousse::timeActivatedFileUpdate::execute()
{
  if (active_) {
    updateFile();
  }
}


void mousse::timeActivatedFileUpdate::end()
{
  if (active_) {
    execute();
  }
}


void mousse::timeActivatedFileUpdate::timeSet()
{
  // Do nothing
}


void mousse::timeActivatedFileUpdate::write()
{
  // Do nothing
}

