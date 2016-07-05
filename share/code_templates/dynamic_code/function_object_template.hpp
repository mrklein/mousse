// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   function_object_template.cpp
#ifndef function_object_template_hpp_
#define function_object_template_hpp_
#include "string_list.hpp"
#include "point_field.hpp"
//{{{ begin codeInclude
${codeInclude}
//}}} end codeInclude
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fvMesh;
class ${typeName}FunctionObject
{
  // Private data
    //- Name of this set of system calls
    word name_;
    //- Registry
    const objectRegistry& obr_;
//{{{ begin codeData
    ${codeData}
//}}} end codeData
  // Private Member Functions
    const objectRegistry& obr() const;
    const fvMesh& mesh() const;
    //- Disallow default bitwise copy construct
    ${typeName}FunctionObject(const ${typeName}FunctionObject&);
    //- Disallow default bitwise assignment
    void operator=(const ${typeName}FunctionObject&);
public:
  //- Runtime type information
  TypeName("${typeName}");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    ${typeName}FunctionObject
    (
      const word& name,
      const objectRegistry& unused,
      const dictionary&,
      const bool loadFromFilesUnused = false
    );
  //- Destructor
  virtual ~${typeName}FunctionObject();
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
    //- Write, execute the "writeCalls"
    virtual void write();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#endif
