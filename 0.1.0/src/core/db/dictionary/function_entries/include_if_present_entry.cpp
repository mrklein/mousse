// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "include_if_present_entry.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "primitive_entry.hpp"


// Static Data Members
const mousse::word mousse::functionEntries::includeIfPresentEntry::typeName
(
  mousse::functionEntries::includeIfPresentEntry::typeName_()
);
// Don't lookup the debug switch here as the debug switch dictionary
// might include includeIfPresentEntry
int mousse::functionEntries::includeIfPresentEntry::debug(0);


namespace mousse {
namespace functionEntries {
ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  includeIfPresentEntry,
  execute,
  dictionaryIstream
);
ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  includeIfPresentEntry,
  execute,
  primitiveEntryIstream
);

}
}


// Member Functions
bool mousse::functionEntries::includeIfPresentEntry::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  const fileName fName{includeFileName(is, parentDict)};
  IFstream ifs{fName};
  if (ifs) {
    if (mousse::functionEntries::includeEntry::report) {
      Info << fName << endl;
    }
    parentDict.read(ifs);
  }
  return true;
}


bool mousse::functionEntries::includeIfPresentEntry::execute
(
  const dictionary& parentDict,
  primitiveEntry& entry,
  Istream& is
)
{
  const fileName fName{includeFileName(is, parentDict)};
  IFstream ifs{fName};
  if (ifs) {
    if (mousse::functionEntries::includeEntry::report) {
      Info << fName << endl;
    }
    entry.read(parentDict, ifs);
  }
  return true;
}
