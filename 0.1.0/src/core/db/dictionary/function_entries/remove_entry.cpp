// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "remove_entry.hpp"
#include "dictionary.hpp"
#include "string_list_ops.hpp"
#include "istring_stream.hpp"
#include "ostring_stream.hpp"
#include "add_to_member_function_selection_table.hpp"


// Static Data Members
const mousse::word mousse::functionEntries::removeEntry::typeName
(
  mousse::functionEntries::removeEntry::typeName_()
);

// Don't lookup the debug switch here as the debug switch dictionary
// might include removeEntry
int mousse::functionEntries::removeEntry::debug(0);


namespace mousse {
namespace functionEntries {

ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  removeEntry,
  execute,
  dictionaryIstream
);

}
}


// Member Functions 
bool mousse::functionEntries::removeEntry::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  wordList dictKeys = parentDict.toc();
  wordReList patterns = readList<wordRe>(is);
  labelList indices = findStrings(patterns, dictKeys);
  FOR_ALL(indices, indexI) {
    parentDict.remove(dictKeys[indices[indexI]]);
  }
  return true;
}
