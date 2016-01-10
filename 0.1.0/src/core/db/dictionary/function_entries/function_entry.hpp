// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntry
// Description
//   A functionEntry causes entries to be added/manipulated on the specified
//   dictionary given an input stream.
//   In dictionaries, a \c '\#' sigil is typically used for a functionEntry.
// SourceFiles
//   function_entry.cpp
//   function_entry_io.cpp

#ifndef function_entry_hpp_
#define function_entry_hpp_

#include "word.hpp"
#include "member_function_selection_tables.hpp"

namespace mousse
{

class primitiveEntry;
class dictionary;

class functionEntry
{
public:
  // Constructors

    //- Disallow default bitwise copy construct
    functionEntry(const functionEntry&) = delete;

    //- Disallow default bitwise assignment
    functionEntry& operator=(const functionEntry&) = delete;

  // Member Function Selectors

    DECLARE_MEMBER_FUNCTION_SELECTION_TABLE
    (
      bool,
      functionEntry,
      execute,
      dictionaryIstream,
      (
        dictionary& parentDict,
        Istream& is
      ),
      (parentDict, is)
    );

    //- Execute the functionEntry in a sub-dict context
    static bool execute
    (
      const word& functionName,
      dictionary& parentDict,
      Istream&
    );

    DECLARE_MEMBER_FUNCTION_SELECTION_TABLE
    (
      bool,
      functionEntry,
      execute,
      primitiveEntryIstream,
      (
        const dictionary& parentDict,
        primitiveEntry& entry,
        Istream& is
      ),
      (parentDict, entry, is)
    );

    //- Execute the functionEntry in a primitiveEntry context
    static bool execute
    (
      const word& functionName,
      const dictionary& parentDict,
      primitiveEntry&,
      Istream&
    );

};

}  // namespace mousse
#endif
