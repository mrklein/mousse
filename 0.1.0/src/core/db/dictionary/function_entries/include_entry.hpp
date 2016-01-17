// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::includeEntry
// Description
//   Specify an include file when reading dictionaries, expects a
//   single string to follow.
//   An example of the \c \#include directive:
//   \verbatim
//     #include "include_file"
//   \endverbatim
//   The usual expansion of environment variables and other constructs
//   (eg, the \c ~OpenFOAM/ expansion) is retained.
// See Also
//   fileName, string::expand()
// SourceFiles
//   include_entry.cpp

#ifndef include_entry_hpp_
#define include_entry_hpp_

#include "function_entry.hpp"

namespace mousse
{

namespace functionEntries
{

class includeEntry
:
  public functionEntry
{
protected:

  // Protected Member Functions

    //- Read the include fileName from Istream, expand and return
    static fileName includeFileName(Istream&, const dictionary&);

    //- Expand include fileName and return
    static fileName includeFileName
    (
      const fileName& dir,
      const fileName&,
      const dictionary&
    );

public:

  // Static data members

    //- Report which file is included to stdout
    static bool report;

  //- Runtime type information
  CLASS_NAME("include");

  // Constructors

    //- Disallow default bitwise copy construct
    includeEntry(const includeEntry&) = delete;

    //- Disallow default bitwise assignment
    includeEntry& operator=(const includeEntry&) = delete;

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
