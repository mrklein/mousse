// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldCoordinateSystemTransform
// Group
//   grpFieldFunctionObjects
// Description
//   This function object transforms a user-specified selection of fields from
//   global Cartesian co-ordinates to a local co-ordinate system.  The fields
//   are run-time modifiable.
//   Example of function object specification:
//   \verbatim
//   fieldCoordinateSystemTransform1
//   {
//     type        fieldCoordinateSystemTransform;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     fields
//     (
//       U
//       UMean
//       UPrime2Mean
//     );
//     coordinateSystem
//     {
//       origin  (0.001  0       0);
//       e1      (1      0.15    0);
//       e3      (0      0      -1);
//     }
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: fieldCoordinateSystemTransform | yes |
//     fields       | list of fields to be transformed |yes |
//     coordinateSystem | local co-ordinate system | yes    |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
//   mousse::coordinateSystem
// SourceFiles
//   field_coordinate_system_transform.cpp
//   field_coordinate_system_transform_templates.cpp
//   i_ofield_coordinate_system_transform.hpp
#ifndef field_coordinate_system_transform_hpp_
#define field_coordinate_system_transform_hpp_
#include "ofstream.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "coordinate_system.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fieldCoordinateSystemTransform
{
protected:
  // Protected data
    //- Name
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Fields to transform
    wordList fieldSet_;
    //- Co-ordinate system to transform to
    coordinateSystem coordSys_;
  // Protected Member Functions
    //- Disallow default bitwise copy construct
    fieldCoordinateSystemTransform(const fieldCoordinateSystemTransform&);
    //- Disallow default bitwise assignment
    void operator=(const fieldCoordinateSystemTransform&);
    template<class Type>
    void transform(const word& fieldName) const;
    template<class Type>
    void transformField(const Type& field) const;
public:
  //- Runtime type information
  TYPE_NAME("fieldCoordinateSystemTransform");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    fieldCoordinateSystemTransform
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~fieldCoordinateSystemTransform();
  // Member Functions
    //- Return name of the fieldCoordinateSystemTransform object
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
#ifdef NoRepository
#   include "field_coordinate_system_transform_templates.cpp"
#endif
#endif
