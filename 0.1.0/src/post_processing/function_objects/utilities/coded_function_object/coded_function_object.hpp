#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_CODED_FUNCTION_OBJECT_CODED_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_CODED_FUNCTION_OBJECT_CODED_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::codedFunctionObject
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object provides a general interface to enable dynamic code
//   compilation.
//   The entries are
//     code        : c++; upon functionObject::write()
//     codeInclude : include files
//     codeOptions : include paths; inserted into EXE_INC in Make/options
//     codeLibs    : link line; inserted into LIB_LIBS in Make/options
//     codeExecute : c++;upon functionObject::execute();
//     codeRead    : c++; upon functionObject::read();
//     codeEnd     : c++; upon functionObject::end();
//     codeData    : c++; local member data (null constructed);
//     codeTimeSet : c++; upon functionObject::timeSet();
//     localCode   : c++; local static functions
//   Example of function object specification:
//   \verbatim
//   difference
//   {
//     functionObjectLibs ("libutilityFunctionObjects.so");
//     type coded;
//     // Name of on-the-fly generated functionObject
//     redirectType writeMagU;
//     code
//     #{
//       // Lookup U
//       const volVectorField& U = mesh().lookupObject<volVectorField>("U");
//       // Write
//       mag(U).write();
//     }
//   }
//   \endverbatim
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
//   mousse::codedBase
// SourceFiles
//   coded_function_object.cpp
#include "function_object.hpp"
#include "coded_base.hpp"
namespace mousse
{
class codedFunctionObject
:
  public functionObject,
  public codedBase
{
protected:
  // Protected data
    //- Reference to the time database
    const Time& time_;
    //- Input dictionary
    dictionary dict_;
    word redirectType_;
    string codeData_;
    string codeRead_;
    string codeExecute_;
    string codeEnd_;
    string codeTimeSet_;
    //- Underlying functionObject
    mutable autoPtr<functionObject> redirectFunctionObjectPtr_;
  // Protected Member Functions
    //- Get the loaded dynamic libraries
    virtual dlLibraryTable& libs() const;
    //- Adapt the context for the current object
    virtual void prepare(dynamicCode &,const dynamicCodeContext&) const;
    // Return a description (type + name) for the output
    virtual string description() const;
    // Clear any redirected objects
    virtual void clearRedirect() const;
    // Get the dictionary to initialize the codeContext
    virtual const dictionary& codeDict() const;
public:
  //- Runtime type information
  TYPE_NAME("coded");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    codedFunctionObject
    (
      const word& name,
      const Time& time,
      const dictionary& dict,
      bool readNow=true   // allow child-classes to avoid compilation
    );
    //- Disallow default bitwise copy construct
    codedFunctionObject(const codedFunctionObject&) = delete;
    //- Disallow default bitwise assignment
    codedFunctionObject& operator=(const codedFunctionObject&) = delete;
  //- Destructor
  virtual ~codedFunctionObject();
  // Member Functions
    //- Dynamically compiled functionObject
    functionObject& redirectFunctionObject() const;
    //- Called at the start of the time-loop
    virtual bool start();
    //- Called at each ++ or += of the time-loop. forceWrite overrides the
    //  outputControl behaviour.
    virtual bool execute(const bool forceWrite);
    //- Called when Time::run() determines that the time-loop exits.
    //  By default it simply calls execute().
    virtual bool end();
    //- Called when time was set at the end of the Time::operator++
    virtual bool timeSet();
    //- Read and set the function object if its data have changed
    virtual bool read(const dictionary&);
    //- Update mesh
    virtual void updateMesh(const mapPolyMesh&);
    //- Move points
    virtual void movePoints(const polyMesh&);
};
}  // namespace mousse
#endif
