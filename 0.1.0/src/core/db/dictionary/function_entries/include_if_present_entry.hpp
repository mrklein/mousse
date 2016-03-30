#ifndef CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INCLUDE_IF_PRESENT_ENTRY_HPP_
#define CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INCLUDE_IF_PRESENT_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::includeIfPresentEntry
// Description
//   Specify a file to include if it exists. Expects a single string to follow.
//   The \c \#includeIfPresent directive is similar to the \c \#include
//   directive, but does not generate an error if the file does not exist.
// See Also
//   mousse::functionEntries::includeEntry

#include "include_entry.hpp"


namespace mousse {
namespace functionEntries {

class includeIfPresentEntry
:
  public includeEntry
{
public:

  //- Runtime type information
  CLASS_NAME("includeIfPresent");

  // Constructors

    //- Disallow default bitwise copy construct
    includeIfPresentEntry(const includeIfPresentEntry&) = delete;

    //- Disallow default bitwise assignment
    includeIfPresentEntry& operator=(const includeIfPresentEntry&) = delete;

  // Member Functions

    //- Execute the functionEntry in a sub-dict context
    static bool execute(dictionary& parentDict, Istream&);

    //- Execute the functionEntry in a primitiveEntry context
    static bool execute
    (
      const dictionary& parentDict,
      primitiveEntry&,
      Istream&
    );

};

}  // namespace functionEntries

}  // namespace mousse

#endif
