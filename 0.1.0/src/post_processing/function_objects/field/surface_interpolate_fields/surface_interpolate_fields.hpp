// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceInterpolateFields
// Group grpFieldFunctionObjects
// Description
//   This function object linearly interpolates volume fields to generate
//   surface fields
//   Fields are stored
//   - every time step the field is updated with new values
//   - at output it writes the fields
//   This functionObject can either be used
//   - to calculate a new field as a  post-processing step or
//   - since the fields are registered, used in another functionObject
//   Example of function object specification:
//   \verbatim
//   surfaceInterpolateFields1
//   {
//     type        surfaceInterpolateFields;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     fields      ((p pNear)(U UNear));
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property | Description               | Required    | Default value
//     type     | type name: nearWallFields | yes         |
//     fields   | list of fields with correspoding output field names | yes |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   surface_interpolate_fields.cpp
//   i_osurface_interpolate_fields.hpp
#ifndef surface_interpolate_fields_hpp_
#define surface_interpolate_fields_hpp_
#include "ofstream.hpp"
#include "surface_fields.hpp"
#include "tuple2.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class surfaceInterpolateFields
{
protected:
  // Protected data
    //- Name of this set of surfaceInterpolateFields object
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Fields to process
    //wordList fieldSet_;
    List<Tuple2<word, word> > fieldSet_;
    //- Locally constructed fields
    PtrList<surfaceScalarField> ssf_;
    PtrList<surfaceVectorField> svf_;
    PtrList<surfaceSphericalTensorField> sSpheretf_;
    PtrList<surfaceSymmTensorField> sSymmtf_;
    PtrList<surfaceTensorField> stf_;
  // Protected Member Functions
    template<class Type>
    void interpolateFields
    (
      PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("surfaceInterpolateFields");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    surfaceInterpolateFields
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    surfaceInterpolateFields(const surfaceInterpolateFields&) = delete;
    //- Disallow default bitwise assignment
    surfaceInterpolateFields& operator=
    (
      const surfaceInterpolateFields&
    ) = delete;
  //- Destructor
  virtual ~surfaceInterpolateFields();
  // Member Functions
    //- Return name of the surfaceInterpolateFields object
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
#ifdef NoRepository
#   include "surface_interpolate_fields_templates.cpp"
#endif
#endif
