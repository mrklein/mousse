// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_entry.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "dictionary.hpp"
#include "dynamic_code.hpp"
#include "code_stream.hpp"
#include "ostring_stream.hpp"
#include "istring_stream.hpp"
#include "primitive_entry.hpp"


// Static Data Members
namespace mousse {
namespace functionEntries {

DEFINE_TYPE_NAME_AND_DEBUG(calcEntry, 0);
ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  calcEntry,
  execute,
  dictionaryIstream
);
ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  calcEntry,
  execute,
  primitiveEntryIstream
);

}
}

// Member Functions
bool mousse::functionEntries::calcEntry::execute
(
  const dictionary& parentDict,
  primitiveEntry& thisEntry,
  Istream& is
)
{
  Info << "Using #calcEntry at line " << is.lineNumber()
    << " in file " <<  parentDict.name() << endl;
  dynamicCode::checkSecurity
  (
    "functionEntries::calcEntry::execute(..)",
    parentDict
  );
  // Read string
  string s{is};
  // Make sure we stop this entry
  //is.putBack(token(token::END_STATEMENT, is.lineNumber()));
  // Construct codeDict for codeStream
  // must reference parent for stringOps::expand to work nicely.
  dictionary codeSubDict;
  codeSubDict.add("code", "os << (" + s + ");");
  dictionary codeDict{parentDict, codeSubDict};
  codeStream::streamingFunctionType function = codeStream::getFunction
  (
    parentDict,
    codeDict
  );
  // use function to write stream
  OStringStream os{is.format()};
  (*function)(os, parentDict);
  // get the entry from this stream
  IStringStream resultStream(os.str());
  thisEntry.read(parentDict, resultStream);
  return true;
}


bool mousse::functionEntries::calcEntry::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  Info << "Using #calcEntry at line " << is.lineNumber()
    << " in file " <<  parentDict.name() << endl;
  dynamicCode::checkSecurity
  (
    "functionEntries::calcEntry::execute(..)",
    parentDict
  );
  // Read string
  string s{is};
  // Make sure we stop this entry
  //is.putBack(token(token::END_STATEMENT, is.lineNumber()));
  // Construct codeDict for codeStream
  // must reference parent for stringOps::expand to work nicely.
  dictionary codeSubDict;
  codeSubDict.add("code", "os << (" + s + ");");
  dictionary codeDict{parentDict, codeSubDict};
  codeStream::streamingFunctionType function = codeStream::getFunction
  (
    parentDict,
    codeDict
  );
  // use function to write stream
  OStringStream os{is.format()};
  (*function)(os, parentDict);
  // get the entry from this stream
  IStringStream resultStream{os.str()};
  parentDict.read(resultStream);
  return true;
}
