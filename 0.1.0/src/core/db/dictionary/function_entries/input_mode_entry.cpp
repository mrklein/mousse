// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "input_mode_entry.hpp"
#include "dictionary.hpp"
#include "add_to_member_function_selection_table.hpp"
// Static Data Members
const mousse::word mousse::functionEntries::inputModeEntry::typeName
(
  mousse::functionEntries::inputModeEntry::typeName_()
);
// Don't lookup the debug switch here as the debug switch dictionary
// might include inputModeEntries
int mousse::functionEntries::inputModeEntry::debug(0);
mousse::functionEntries::inputModeEntry::inputMode
  mousse::functionEntries::inputModeEntry::mode_(MERGE);
namespace mousse
{
namespace functionEntries
{
  ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
  (
    functionEntry,
    inputModeEntry,
    execute,
    dictionaryIstream
  );
}
}
// Private Member Functions 
// we could combine this into execute() directly, but leave it here for now
void mousse::functionEntries::inputModeEntry::setMode(Istream& is)
{
  clear();
  word mode(is);
  if (mode == "merge" || mode == "default")
  {
    mode_ = MERGE;
  }
  else if (mode == "overwrite")
  {
    mode_ = OVERWRITE;
  }
  else if (mode == "protect")
  {
    mode_ = PROTECT;
  }
  else if (mode == "warn")
  {
    mode_ = WARN;
  }
  else if (mode == "error")
  {
    mode_ = ERROR;
  }
  else
  {
    WARNING_IN("mousse::functionEntries::inputModeEntry::setMode(Istream&)")
      << "unsupported input mode '" << mode
      << "' ... defaulting to 'merge'"
      << endl;
  }
}
// Member Functions 
bool mousse::functionEntries::inputModeEntry::execute
(
  dictionary& /*parentDict*/,
  Istream& is
)
{
  setMode(is);
  return true;
}
void mousse::functionEntries::inputModeEntry::clear()
{
  mode_ = MERGE;
}
bool mousse::functionEntries::inputModeEntry::merge()
{
  return mode_ == MERGE;
}
bool mousse::functionEntries::inputModeEntry::overwrite()
{
  return mode_ == OVERWRITE;
}
bool mousse::functionEntries::inputModeEntry::protect()
{
  return mode_ == PROTECT;
}
bool mousse::functionEntries::inputModeEntry::error()
{
  return mode_ == ERROR;
}
