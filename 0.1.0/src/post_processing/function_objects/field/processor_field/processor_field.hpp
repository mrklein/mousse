// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::processorField
// Group
//   grpFieldFunctionObjects
// Description
//   This function object writes a scalar field whose value is the local
//   processor ID.  The output field name is 'processorID'.
//   Example of function object specification:
//   \verbatim
//   processorField1
//   {
//     type        processorField;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: processorField | yes       |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   processor_field.cpp
//   i_oprocessor_field.hpp
#ifndef processor_field_hpp_
#define processor_field_hpp_
#include "ofstream.hpp"
#include "point_field_fwd.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "coordinate_system.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class processorField
{
protected:
  // Protected data
    //- Name of this set of nearWallFields object
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
  // Protected Member Functions
    //- Disallow default bitwise copy construct
    processorField(const processorField&);
    //- Disallow default bitwise assignment
    void operator=(const processorField&);
public:
  //- Runtime type information
  TYPE_NAME("processorField");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    processorField
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~processorField();
  // Member Functions
    //- Return name of the processorField object
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the input data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write
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
