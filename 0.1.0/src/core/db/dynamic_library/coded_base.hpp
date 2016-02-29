#ifndef CORE_DB_DYNAMIC_LIBRARY_CODED_BASE_HPP_
#define CORE_DB_DYNAMIC_LIBRARY_CODED_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::codedBase
// Description
//   Base class for function objects and boundary conditions using dynamic code
// SourceFiles
//   coded_base.cpp


#include "dictionary.hpp"

namespace mousse
{

// Forward declaration of classes
class dynamicCode;
class dynamicCodeContext;
class dlLibraryTable;

class codedBase
{
  // Private data

    //- Previously loaded library
    mutable fileName oldLibPath_;

  // Private Member Functions

    //- Global loader/unloader function type
    typedef void (*loaderFunctionType)(bool);

    //- Load specified library and execute globalFuncName(true)
    void* loadLibrary
    (
      const fileName& libPath,
      const string& globalFuncName,
      const dictionary& contextDict
    ) const;

    //- Execute globalFuncName(false) and unload specified library
    void unloadLibrary
    (
      const fileName& libPath,
      const string& globalFuncName,
      const dictionary& contextDict
    ) const;

    //- Create library based on the dynamicCodeContext
    void createLibrary(dynamicCode&, const dynamicCodeContext&) const;

protected:

    //- Update library as required
    void updateLibrary
    (
      const word& redirectType
    ) const;

    //- Get the loaded dynamic libraries
    virtual dlLibraryTable& libs() const = 0;

    //- Adapt the context for the current object
    virtual void prepare
    (
      dynamicCode&,
      const dynamicCodeContext&
    ) const = 0;

    // Return a description (type + name) for the output
    virtual string description() const = 0;

    // Clear any redirected objects
    virtual void clearRedirect() const = 0;

    // Get the dictionary to initialize the codeContext
    virtual const dictionary& codeDict() const = 0;

public:

  //- Runtime type information
  CLASS_NAME("codedBase");

  // Constructors

    //- Construct null
    codedBase();

    //- Disallow default bitwise copy construct
    codedBase(const codedBase&) = delete;

    //- Disallow default bitwise assignment
    void operator=(const codedBase&) = delete;

  //- Destructor
  virtual ~codedBase();
};

}  // namespace mousse

#endif
