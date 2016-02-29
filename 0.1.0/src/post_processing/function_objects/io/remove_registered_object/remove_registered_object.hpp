#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_REMOVE_REGISTERED_OBJECT_REMOVE_REGISTERED_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_REMOVE_REGISTERED_OBJECT_REMOVE_REGISTERED_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::removeRegisteredObject
// Group
//   grpIOFunctionObjects
// Description
//   This function object removes registered objects if present in the database
//   Example of function object specification:
//   \verbatim
//   removeRegisteredObject1
//   {
//     type        removeRegisteredObject;
//     functionObjectLibs ("libIOFunctionObjects.so");
//     ...
//     objectNames (obj1 obj2);
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: removeRegisteredObject | yes |
//     objectNames  | objects to remove       | yes         |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   remove_registered_object.cpp
//   i_oremove_registered_object.hpp
#include "word_list.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class removeRegisteredObject
{
protected:
  // Private data
    //- Name of this set of removeRegisteredObject
    word name_;
    const objectRegistry& obr_;
    // Read from dictionary
      //- Names of objects to control
      wordList objectNames_;
public:
  //- Runtime type information
  TYPE_NAME("removeRegisteredObject");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    removeRegisteredObject
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    removeRegisteredObject(const removeRegisteredObject&) = delete;
    //- Disallow default bitwise assignment
    removeRegisteredObject& operator=(const removeRegisteredObject&) = delete;
  //- Destructor
  virtual ~removeRegisteredObject();
  // Member Functions
    //- Return name of the removeRegisteredObject
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the removeRegisteredObject data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write the removeRegisteredObject
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
