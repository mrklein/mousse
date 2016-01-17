// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::calcEntry
// Description
//   Uses dynamic compilation to provide calculating functionality
//   for entering dictionary entries.
//   E.g.
//   \verbatim
//   a 1.0;
//   b 3;
//   c #calc "$a/$b";
//   \endverbatim
//   Note the explicit trailing 0 ('1.0') to force a to be read (and written)
//   as a floating point number.
// Note
//   Internally this is just a wrapper around codeStream functionality - the
//   #calc string gets used to construct a dictionary for codeStream.
// SourceFiles
//   calc_entry.cpp

#ifndef calc_entry_hpp_
#define calc_entry_hpp_

#include "function_entry.hpp"

namespace mousse
{

class dlLibraryTable;

namespace functionEntries
{
class calcEntry
:
  public functionEntry
{
public:
  //- Runtime type information
  CLASS_NAME("calc");

  // Constructors

    //- Disallow default bitwise copy construct
    calcEntry(const calcEntry&) = delete;

    //- Disallow default bitwise assignment
    calcEntry& operator=(const calcEntry&) = delete;

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
