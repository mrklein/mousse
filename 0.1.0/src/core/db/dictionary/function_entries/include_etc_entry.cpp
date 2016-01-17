// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "include_etc_entry.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "string_ops.hpp"
#include "os_specific.hpp"
#include "primitive_entry.hpp"

// Static Data Members
const mousse::word mousse::functionEntries::includeEtcEntry::typeName
(
  mousse::functionEntries::includeEtcEntry::typeName_()
);
// Don't lookup the debug switch here as the debug switch dictionary
// might include includeEtcEntry
int mousse::functionEntries::includeEtcEntry::debug(0);
bool mousse::functionEntries::includeEtcEntry::report(false);
namespace mousse
{
namespace functionEntries
{
  ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
  (
    functionEntry,
    includeEtcEntry,
    execute,
    dictionaryIstream
  );

  ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
  (
    functionEntry,
    includeEtcEntry,
    execute,
    primitiveEntryIstream
  );
}
}
// Protected Member Functions
mousse::fileName mousse::functionEntries::includeEtcEntry::includeEtcFileName
(
  const fileName& f,
  const dictionary& dict
)
{
  fileName fName(f);
  // Substitute dictionary and environment variables.
  // Allow empty substitutions.
  stringOps::inplaceExpand(fName, dict, true, true);
  if (fName.empty() || fName.isAbsolute())
  {
    return fName;
  }
  else
  {
    // Search the etc directories for the file
    return findEtcFile(fName);
  }
}

// Member Functions
bool mousse::functionEntries::includeEtcEntry::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  const fileName rawFName(is);
  const fileName fName
  (
    includeEtcFileName(rawFName, parentDict)
  );
  IFstream ifs(fName);
  if (ifs)
  {
    if (mousse::functionEntries::includeEtcEntry::report)
    {
      Info<< fName << endl;
    }
    parentDict.read(ifs);
    return true;
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::includeEtcEntry::includeEtcEntry"
      "(dictionary& parentDict, Istream&)",
      is
    )   << "Cannot open etc file "
      << (ifs.name().size() ? ifs.name() : rawFName)
      << " while reading dictionary " << parentDict.name()
      << exit(FatalIOError);
    return false;
  }
}
bool mousse::functionEntries::includeEtcEntry::execute
(
  const dictionary& parentDict,
  primitiveEntry& entry,
  Istream& is
)
{
  const fileName rawFName(is);
  const fileName fName
  (
    includeEtcFileName(rawFName, parentDict)
  );
  IFstream ifs(fName);
  if (ifs)
  {
    if (mousse::functionEntries::includeEtcEntry::report)
    {
      Info<< fName << endl;
    }
    entry.read(parentDict, ifs);
    return true;
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::includeEtcEntry::includeEtcEntry"
      "(dictionary& parentDict, primitiveEntry&, Istream&)",
      is
    )   << "Cannot open etc file "
      << (ifs.name().size() ? ifs.name() : rawFName)
      << " while reading dictionary " << parentDict.name()
      << exit(FatalIOError);
    return false;
  }
}
