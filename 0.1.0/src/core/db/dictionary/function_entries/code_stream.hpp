// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::functionEntries::codeStream
// Description
//   Dictionary entry that contains C++ OpenFOAM code that is compiled to
//   generate the entry itself. So
//   - codeStream reads three entries: 'code', 'codeInclude' (optional),
//   'codeOptions' (optional)
//   and uses those to generate library sources inside \c codeStream/
//   - these get compiled using 'wmake libso'
//   - the resulting library is loaded in executed with as arguments
//   \code
//     (const dictionary& dict, Ostream& os)
//   \endcode
//   where the dictionary is the current dictionary.
//   - the code has to write into Ostream which is then used to construct
//   the actual dictionary entry.
//   E.g. to set the internal field of a field:
//   \verbatim
//   internalField  #codeStream
//   {
//     code
//     #{
//       const IOdictionary& d = static_cast<const IOdictionary&>(dict);
//       const fvMesh& mesh = refCast<const fvMesh>(d.db());
//       scalarField fld(mesh.nCells(), 12.34);
//       fld.writeEntry("", os);
//     #};
//     //- Optional:
//     codeInclude
//     #{
//       #include "fv_cfd.hpp"
//     #};
//     //- Optional:
//     codeOptions
//     #{
//       -I$(LIB_SRC)/finiteVolume/lnInclude
//     #};
//   };
//   \endverbatim
//   Note the \c \#{ ... \c \#} syntax is a 'verbatim' input mode that allows
//   inputting strings with embedded newlines.
//   Limitations:
//   - '~' symbol not allowed inside the code sections.
//   - probably some other limitations (uses string::expand which expands
//    \c \$ and \c ~ sequences)
// Note
//   The code to be compiled is stored under the local \c codeStream directory
//   with a subdirectory name corresponding to the SHA1 of the contents.
//   The corresponding library code is located under the local
//   \c codeStream/platforms/$WM_OPTIONS/lib directory in a library
//   \c libcodeStream_SHA1.so
// SourceFiles
//   code_stream.cpp

#ifndef code_stream_hpp_
#define code_stream_hpp_

#include "function_entry.hpp"

namespace mousse
{

class dlLibraryTable;

namespace functionEntries
{

// Forward declaration of friend classes
class calcEntry;

class codeStream
:
  public functionEntry
{
    //- Interpreter function type
    typedef void (*streamingFunctionType)(Ostream&, const dictionary&);

  // Private Member Functions

    //- Helper function: parent (of parent etc.) of dictionary up to the top
    static const dictionary& topDict(const dictionary&);

    //- Helper function: access to dlLibraryTable of Time
    static dlLibraryTable& libs(const dictionary& dict);

    //- Construct, compile, load and return streaming function
    static streamingFunctionType getFunction
    (
      const dictionary& parentDict,
      const dictionary& codeDict
    );

public:

  // Static data members

    //- Name of the C code template to be used
    static const word codeTemplateC;

  // Related types

    //- Declare friendship with the calcEntry class
    friend class calcEntry;

  //- Runtime type information
  CLASS_NAME("codeStream");

  // Constructors

    //- Disallow default bitwise copy construct
    codeStream(const codeStream&) = delete;

    //- Disallow default bitwise assignment
    codeStream& operator=(const codeStream&) = delete;

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
