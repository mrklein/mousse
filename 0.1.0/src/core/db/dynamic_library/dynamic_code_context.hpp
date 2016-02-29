#ifndef CORE_DB_DYNAMIC_LIBRARY_DYNAMIC_CODE_CONTEXT_HPP_
#define CORE_DB_DYNAMIC_LIBRARY_DYNAMIC_CODE_CONTEXT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicCodeContext
// Description
//   Encapsulation of dynamic code dictionaries
// SourceFiles
//   dynamic_code_context.cpp
#include "dictionary.hpp"
#include "sha1_digest.hpp"
namespace mousse
{
class dynamicCodeContext
{
  // Private data
    //- The parent dictionary context
    const dictionary& dict_;
    //- Mandatory "code" entry
    string code_;
    //- Optional "localCode" entry
    string localCode_;
    //- Optional "codeInclude" entry
    string include_;
    //- Optional "codeOptions" entry
    string options_;
    //- Optional "codeLib" entry
    string libs_;
    //- Calculated SHA1Digest
    SHA1Digest sha1_;
public:
  // Constructors
    //- Construct from a dictionary
    dynamicCodeContext(const dictionary&);
  // Member functions
    //- Return the parent dictionary context
    const dictionary& dict() const
    {
      return dict_;
    }
    //- Return the code-includes
    const string& include() const
    {
      return include_;
    }
    //- Return the code-options
    const string& options() const
    {
      return options_;
    }
    //- Return the code-libs
    const string& libs() const
    {
      return libs_;
    }
    //- Return the code
    const string& code() const
    {
      return code_;
    }
    //- Return the local (file-scope) code
    const string& localCode() const
    {
      return localCode_;
    }
    //- Return SHA1 digest calculated from include, options, code
    const SHA1Digest& sha1() const
    {
      return sha1_;
    }
    //- Helper: add \#line directive
    static void addLineDirective
    (
      string&,
      const label lineNum,
      const fileName& name
    );
};
}  // namespace mousse
#endif
