// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "include_entry.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "string_ops.hpp"
#include "primitive_entry.hpp"


// Static Data Members
const mousse::word mousse::functionEntries::includeEntry::typeName
(
  mousse::functionEntries::includeEntry::typeName_()
);

// Don't lookup the debug switch here as the debug switch dictionary
// might include includeEntry
int mousse::functionEntries::includeEntry::debug(0);
bool mousse::functionEntries::includeEntry::report(false);


namespace mousse {
namespace functionEntries {

ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  includeEntry,
  execute,
  dictionaryIstream
);
ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  includeEntry,
  execute,
  primitiveEntryIstream
);

}
}


// Protected Member Functions
mousse::fileName mousse::functionEntries::includeEntry::includeFileName
(
  Istream& is,
  const dictionary& dict
)
{
  fileName fName{is};
  // Substitute dictionary and environment variables. Allow empty
  // substitutions.
  stringOps::inplaceExpand(fName, dict, true, true);
  if (fName.empty() || fName.isAbsolute()) {
    return fName;
  } else {
    // relative name
    return fileName(is.name()).path()/fName;
  }
}


mousse::fileName mousse::functionEntries::includeEntry::includeFileName
(
  const fileName& dir,
  const fileName& f,
  const dictionary& dict
)
{
  fileName fName(f);
  // Substitute dictionary and environment variables. Allow empty
  // substitutions.
  stringOps::inplaceExpand(fName, dict, true, true);
  if (fName.empty() || fName.isAbsolute()) {
    return fName;
  } else {
    // relative name
    return dir/fName;
  }
}


// Member Functions
bool mousse::functionEntries::includeEntry::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  const fileName rawFName{is};
  const fileName fName
  {
    includeFileName(is.name().path(), rawFName, parentDict)
  };
  IFstream ifs{fName};
  if (ifs) {
    if (mousse::functionEntries::includeEntry::report) {
      Info << fName << endl;
    }
    parentDict.read(ifs);
    return true;
  } else {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::includeEntry::includeEntry"
      "(dictionary& parentDict, Istream&)",
      is
    )
    << "Cannot open include file "
    << (ifs.name().size() ? ifs.name() : rawFName)
    << " while reading dictionary " << parentDict.name()
    << exit(FatalIOError);
    return false;
  }
}


bool mousse::functionEntries::includeEntry::execute
(
  const dictionary& parentDict,
  primitiveEntry& entry,
  Istream& is
)
{
  const fileName rawFName{is};
  const fileName fName
  {
    includeFileName(is.name().path(), rawFName, parentDict)
  };
  IFstream ifs{fName};
  if (ifs) {
    if (mousse::functionEntries::includeEntry::report) {
      Info << fName << endl;
    }
    entry.read(parentDict, ifs);
    return true;
  } else {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::includeEntry::includeEntry"
      "(dictionary& parentDict, primitiveEntry&, Istream&)",
      is
    )
    << "Cannot open include file "
    << (ifs.name().size() ? ifs.name() : rawFName)
    << " while reading dictionary " << parentDict.name()
    << exit(FatalIOError);
    return false;
  }
}
