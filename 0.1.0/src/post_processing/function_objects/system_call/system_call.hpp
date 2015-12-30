// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::systemCall
// Group
//   grpFunctionObjects
// Description
//   This function object executes system calls, entered in the form of a
//   string lists.  Calls can be made at the following points in the
//   calculation:
//   - every time step
//   - every output time
//   - end of the calculation
//   Example of function object specification:
//   \verbatim
//   systemCall1
//   {
//     type        systemCall;
//     functionObjectLibs ("libsystemCall.so");
//     ...
//     executeCalls
//     (
//       "echo execute"
//     );
//     writeCalls
//     (
//       "echo \*\*\* writing data \*\*\*"
//     );
//     endCalls
//     (
//       "echo \*\*\* writing .bashrc \*\*\*"
//       "cat ~/.bashrc"
//       "echo \*\*\* done \*\*\*"
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: systemCall   | yes         |
//     executeCalls | list of calls on execute | yes        |
//     writeCalls   | list of calls on write  | yes         |
//     endCalls     | list of calls on end    | yes         |
//   \endtable
// Note
//   Since this function object executes system calls, there is a potential
//   security risk.  In order to use the \c systemCall function object, the
//   \c allowSystemOperations must be set to '1'; otherwise, system calls will
//   not be allowed.
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   system_call.cpp
//   i_osystem_call.hpp
#ifndef system_call_hpp_
#define system_call_hpp_
#include "string_list.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class systemCall
{
protected:
  // Private data
    //- Name of this set of system calls
    word name_;
    //- List of calls to execute - every step
    stringList executeCalls_;
    //- List of calls to execute when exiting the time-loop
    stringList endCalls_;
    //- List of calls to execute - write steps
    stringList writeCalls_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    systemCall(const systemCall&);
    //- Disallow default bitwise assignment
    void operator=(const systemCall&);
public:
  //- Runtime type information
  TypeName("systemCall");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    systemCall
    (
      const word& name,
      const objectRegistry& unused,
      const dictionary&,
      const bool loadFromFilesUnused = false
    );
  //- Destructor
  virtual ~systemCall();
  // Member Functions
    //- Return name of the system call set
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the system calls
    virtual void read(const dictionary&);
    //- Execute the "executeCalls" at each time-step
    virtual void execute();
    //- Execute the "endCalls" at the final time-loop
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write, execute the "writeCalls"
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#endif
