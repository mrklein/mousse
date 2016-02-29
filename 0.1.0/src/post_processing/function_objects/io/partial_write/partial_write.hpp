#ifndef POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_PARTIAL_WRITE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_IO_PARTIAL_WRITE_PARTIAL_WRITE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::partialWrite
// Group
//   grpIOFunctionObjects
// Description
//   This function object allows user-selected fields/registered objects to be
//   written at a custom write interval. The interval is given in terms of
//   number of overall dumps
//   Example of function object specification:
//   \verbatim
//   partialWrite1
//   {
//     type        partialWrite;
//     functionObjectLibs ("libIOFunctionObjects.so");
//     ...
//     objectNames (p U T);
//     writeInterval 100;
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: partialWrite | yes         |
//     objectNames  | objects to write        | yes         |
//     writeInterval | write interval         | yes         |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   partial_write.cpp
//   i_opartial_write.hpp
#include "hash_set.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class partialWrite
{
protected:
  // Private data
    //- Name of this set of partialWrite
    word name_;
    const objectRegistry& obr_;
    //- Loaded fields
    UPtrList<volScalarField> vsf_;
    UPtrList<volVectorField> vvf_;
    UPtrList<volSphericalTensorField> vSpheretf_;
    UPtrList<volSymmTensorField> vSymmtf_;
    UPtrList<volTensorField> vtf_;
    UPtrList<surfaceScalarField> ssf_;
    UPtrList<surfaceVectorField> svf_;
    UPtrList<surfaceSphericalTensorField> sSpheretf_;
    UPtrList<surfaceSymmTensorField> sSymmtf_;
    UPtrList<surfaceTensorField> stf_;
    // Read from dictionary
      //- Names of objects to dump always
      HashSet<word> objectNames_;
      //- Write interval for restart dump
      label writeInterval_;
    //- Current dump instance. If reaches writeInterval do a full write.
    label writeInstance_;
  // Private Member Functions
    //- Load objects in the objectNames
    template<class Type>
    void loadField
    (
      const word&,
      UPtrList<GeometricField<Type, fvPatchField, volMesh> >&,
      UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    ) const;
    template<class Type>
    void changeWriteOptions
    (
      UPtrList<GeometricField<Type, fvPatchField, volMesh> >&,
      UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&,
      const IOobject::writeOption
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("partialWrite");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    partialWrite
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    partialWrite(const partialWrite&) = delete;
    //- Disallow default bitwise assignment
    partialWrite& operator=(const partialWrite&) = delete;
  //- Destructor
  virtual ~partialWrite();
  // Member Functions
    //- Return name of the partialWrite
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the partialWrite data
    virtual void read(const dictionary&);
    //- Execute
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write the partialWrite
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
#   include "partial_write_templates.cpp"
#endif
#endif
