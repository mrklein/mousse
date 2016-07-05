#ifndef CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INPUT_MODE_ENTRY_HPP_
#define CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INPUT_MODE_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::inputModeEntry
// Description
//   Specify the input mode when reading dictionaries, expects
//   a single word to follow.
//   An example of \c \#inputMode directive:
//   \verbatim
//     #inputMode merge
//   \endverbatim
//   The possible input modes:
//   \param merge      merge sub-dictionaries when possible
//   \param overwrite  keep last entry and silently remove previous ones
//   \param protect    keep initial entry and silently ignore subsequent ones
//   \param warn       keep initial entry and warn about subsequent ones
//   \param error      issue a FatalError for duplicate entries
//   \param default    currently identical to merge

#include "function_entry.hpp"


namespace mousse {
namespace functionEntries {

class inputModeEntry
:
  public functionEntry
{
    //- The input mode options
    enum inputMode
    {
      MERGE,
      OVERWRITE,
      PROTECT,
      WARN,
      ERROR
    };

    //- The current input mode
    static inputMode mode_;

  // Private Member Functions

    //- Read the mode as a word and set enum appropriately
    static void setMode(Istream&);

public:

  //- Runtime type information
  CLASS_NAME("inputMode");

  // Constructors

    //- Disallow default bitwise copy construct
    inputModeEntry(const inputModeEntry&) = delete;

    //- Disallow default bitwise assignment
    inputModeEntry& operator=(const inputModeEntry&) = delete;

  // Member Functions

    //- Execute the functionEntry in a sub-dict context
    static bool execute(dictionary& parentDict, Istream&);

    //- Reset the inputMode to %default (ie, %merge)
    static void clear();

    //- Return true if the inputMode is %merge
    static bool merge();

    //- Return true if the inputMode is %overwrite
    static bool overwrite();

    //- Return true if the inputMode is %protect
    static bool protect();

    //- Return true if the inputMode is %error
    static bool error();

};

}  // namespace functionEntries

}  // namespace mousse
#endif
