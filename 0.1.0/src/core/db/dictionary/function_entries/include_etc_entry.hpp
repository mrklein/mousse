#ifndef CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INCLUDE_ETC_ENTRY_HPP_
#define CORE_DB_DICTIONARY_FUNCTION_ENTRIES_INCLUDE_ETC_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::includeEtcEntry
// Description
//   Specify an etc file to include when reading dictionaries, expects a
//   single string to follow.
//   Searches for files from user/group/shipped directories.
//   The search scheme allows for version-specific and
//   version-independent files using the following hierarchy:
//   - \b user settings:
//    - ~/.OpenFOAM/\<VERSION\>
//    - ~/.OpenFOAM/
//   - \b group (site) settings (when $WM_PROJECT_SITE is set):
//    - $WM_PROJECT_SITE/\<VERSION\>
//    - $WM_PROJECT_SITE
//   - \b group (site) settings (when $WM_PROJECT_SITE is not set):
//    - $WM_PROJECT_INST_DIR/site/\<VERSION\>
//    - $WM_PROJECT_INST_DIR/site/
//   - \b other (shipped) settings:
//    - $WM_PROJECT_DIR/etc/
//   An example of the \c \#includeEtc directive:
//   \verbatim
//     #includeEtc "etcFile"
//   \endverbatim
//   The usual expansion of environment variables and other constructs is
//   retained.
// See Also
//   findEtcFile, fileName, string::expand()
// SourceFiles
//   include_etc_entry.cpp

#include "function_entry.hpp"


namespace mousse {
namespace functionEntries {

class includeEtcEntry
:
  public functionEntry
{
  // Private Member Functions

    //- Expand include fileName and return
    static fileName includeEtcFileName
    (
      const fileName&,
      const dictionary&
    );

public:

  // Static data members

    //- Report which file is included to stdout
    static bool report;

  //- Runtime type information
  CLASS_NAME("includeEtc");

  // Constructors

    //- Disallow default bitwise copy construct
    includeEtcEntry(const includeEtcEntry&) = delete;

    //- Disallow default bitwise assignment
    includeEtcEntry& operator=(const includeEtcEntry&) = delete;

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
