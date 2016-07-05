#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_READ_FIELDS_READ_FIELDS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_READ_FIELDS_READ_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::readFields
// Group
//   grpFieldFunctionObjects
// Description
//   This function object reads fields from the time directories and adds them to
//   the mesh database for further post-processing.
//   Example of function object specification:
//   \verbatim
//   readFields1
//   {
//     type        readFields;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     fields
//     (
//       U
//       p
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: readFields   | yes         |
//     fields       | list of fields to read  |  no         |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject

#include "ofstream.hpp"
#include "point_field_fwd.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;


class readFields
{
protected:
  // Protected data
    //- Name of this set of readFields object
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Fields to load
    wordList fieldSet_;
    //- Loaded fields
    PtrList<volScalarField> vsf_;
    PtrList<volVectorField> vvf_;
    PtrList<volSphericalTensorField> vSpheretf_;
    PtrList<volSymmTensorField> vSymmtf_;
    PtrList<volTensorField> vtf_;
    PtrList<surfaceScalarField> ssf_;
    PtrList<surfaceVectorField> svf_;
    PtrList<surfaceSphericalTensorField> sSpheretf_;
    PtrList<surfaceSymmTensorField> sSymmtf_;
    PtrList<surfaceTensorField> stf_;
  // Protected Member Functions
    template<class Type>
    void loadField
    (
      const word&,
      PtrList<GeometricField<Type, fvPatchField, volMesh>>&,
      PtrList<GeometricField<Type, fvsPatchField, surfaceMesh>>&
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("readFields");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    readFields
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    readFields(const readFields&) = delete;
    //- Disallow default bitwise assignment
    readFields& operator=(const readFields&) = delete;
  //- Destructor
  virtual ~readFields();
  // Member Functions
    //- Return name of the readFields object
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the field min/max data
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

#include "read_fields.ipp"

#endif
