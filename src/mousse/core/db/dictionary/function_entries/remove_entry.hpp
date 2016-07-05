#ifndef CORE_DB_DICTIONARY_FUNCTION_ENTRIES_REMOVE_ENTRY_HPP_
#define CORE_DB_DICTIONARY_FUNCTION_ENTRIES_REMOVE_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::removeEntry
// Description
//   Remove a dictionary entry.
//   The \c \#remove directive takes a list or a single wordRe.
//   For example,
//   \verbatim
//     #remove entry0
//     #remove ( entry1 entry2 entry3 otherEntry )
//     #remove "entry[1-3]"
//     #remove ( "entry[1-3]" otherEntry )
//   \endverbatim
//   The removal only occurs in the current context.
//   Removing sub-entries or parent entries is not supported.

#include "function_entry.hpp"


namespace mousse {
namespace functionEntries {

class removeEntry
:
  public functionEntry
{
public:

  //- Runtime type information
  CLASS_NAME("remove");

  // Constructors

    //- Disallow default bitwise copy construct
    removeEntry(const removeEntry&) = delete;

    //- Disallow default bitwise assignment
    removeEntry& operator=(const removeEntry&) = delete;

  // Member Functions

    //- Execute the functionEntry in a sub-dict context
    static bool execute(dictionary& parentDict, Istream&);

};

}  // namespace functionEntries

}  // namespace mousse
#endif
