#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FACE_SOURCE_FACE_SOURCE_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_FIELD_VALUES_FACE_SOURCE_FACE_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldValues::faceSource
// Group
//   grpFieldFunctionObjects
// Description
//   This function object provides a 'face source' variant of the fieldValues
//   function object.  Given a list of user-specified fields and a selection
//   of mesh (or general surface) faces, a number of operations can be
//   performed, such as sums, averages and integrations.
//   \linebreak
//   For example, to calculate the volumetric or mass flux across a patch,
//   apply the 'sum' operator to the flux field (typically \c phi)
//   Example of function object specification:
//   \verbatim
//   faceSource1
//   {
//     type            faceSource;
//     functionObjectLibs ("libfieldFunctionObjects.so");
//     ...
//     log             yes;
//     valueOutput     true;
//     surfaceFormat   none;
//     source          faceZone;
//     sourceName      f0;
//     operation       sum;
//     weightField     alpha1;
//     fields
//     (
//       p
//       phi
//       U
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: faceSource   | yes         |
//     log          | write data to standard output | no    | no
//     valueOutput  | write the output values | yes         |
//     writeArea    | Write the area of the faceSource | no |
//     surfaceFormat | output value format    | no          |
//     source       | face source: see below  | yes         |
//     sourceName   | name of face source if required  | no |
//     operation    | operation to perform    | yes         |
//     weightField  | name of field to apply weighting | no |
//     orientedWeightField  | name of oriented field to apply weighting | no |
//     scaleFactor  | scale factor            | no          | 1
//     fields       | list of fields to operate on | yes    |
//     orientedFields | list of oriented fields to operate on | no |
//   \endtable
//   \linebreak
//   Where \c source is defined by
//   \plaintable
//     faceZone     | requires a 'sourceName' entry to specify the faceZone
//     patch        | requires a 'sourceName' entry to specify the patch
//     sampledSurface | requires a 'sampledSurfaceDict' sub-dictionary
//   \endplaintable
//   \linebreak
//   The \c operation is one of:
//   \plaintable
//    none          | no operation
//    sum           | sum
//    sumMag        | sum of component magnitudes
//    sumDirection  | sum values which are positive in given direction
//    sumDirectionBalance | sum of balance of values in given direction
//    average       | ensemble average
//    weightedAverage | weighted average
//    areaAverage   | area weighted average
//    weightedAreaAverage | weighted area average
//    areaIntegrate | area integral
//    min           | minimum
//    max           | maximum
//    CoV           | coefficient of variation: standard deviation/mean
//    areaNormalAverage| area weighted average in face normal direction
//    areaNormalIntegrate | area weighted integral in face normal directon
//   \endplaintable
// Note
//   - The values reported by the areaNormalAverage and areaNormalIntegrate
//    operations are written as the first component of a field with the same
//    rank as the input field.
//   - faces on empty patches get ignored
//   - if the field is a volField the \c faceZone can only consist of boundary
//    faces
//   - the `oriented' entries relate to mesh-oriented fields, such as the
//    flux, phi.  These fields will be oriented according to the face normals.
//   - using \c sampledSurfaces:
//     - not available for surface fields
//     - if interpolate=true they use \c interpolationCellPoint
//      otherwise they use cell values
//     - each triangle in \c sampledSurface is logically only in one cell
//      so interpolation will be wrong when triangles are larger than
//      cells.  This can only happen for sampling on a \c triSurfaceMesh
//     - take care when using isoSurfaces - these might have duplicate
//      triangles and so integration might be wrong
// SeeAlso
//   mousse::fieldValues
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   face_source.cpp
//   face_source_templates.cpp
#include "named_enum.hpp"
#include "field_value.hpp"
#include "surface_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_writer.hpp"
namespace mousse
{
class sampledSurface;
namespace fieldValues
{
class faceSource
:
  public fieldValue
{
public:
  // Public data types
    //- Source type enumeration
    enum sourceType
    {
      stFaceZone,
      stPatch,
      stSampledSurface
    };
    //- Source type names
    static const NamedEnum<sourceType, 3> sourceTypeNames_;
    //- Operation type enumeration
    enum operationType
    {
      opNone,
      opSum,
      opSumMag,
      opSumDirection,
      opSumDirectionBalance,
      opAverage,
      opWeightedAverage,
      opAreaAverage,
      opWeightedAreaAverage,
      opAreaIntegrate,
      opMin,
      opMax,
      opCoV,
      opAreaNormalAverage,
      opAreaNormalIntegrate
    };
    //- Operation type names
    static const NamedEnum<operationType, 15> operationTypeNames_;
private:
  // Private Member Functions
    //- Set faces to evaluate based on a face zone
    void setFaceZoneFaces();
    //- Set faces to evaluate based on a patch
    void setPatchFaces();
    //- Set faces according to sampledSurface
    void sampledSurfaceFaces(const dictionary&);
    //- Combine mesh faces and points from multiple processors
    void combineMeshGeometry
    (
      faceList& faces,
      pointField& points
    ) const;
    //- Combine surface faces and points from multiple processors
    void combineSurfaceGeometry
    (
      faceList& faces,
      pointField& points
    ) const;
    //- Calculate and return total area of the faceSource: sum(magSf)
    scalar totalArea() const;
protected:
  // Protected data
    //- Surface writer
    autoPtr<surfaceWriter> surfaceWriterPtr_;
    //- Source type
    sourceType source_;
    //- Operation to apply to values
    operationType operation_;
    //- Weight field name - optional
    word weightFieldName_;
    //- Flag to indicate if flipMap should be applied to the weight field
    bool orientWeightField_;
    //- Start index of fields that require application of flipMap
    label orientedFieldsStart_;
    //- Scale factor - optional
    scalar scaleFactor_;
    //- Total area of the faceSource
    scalar totalArea_;
    //- Optionally write the area of the faceSource
    bool writeArea_;
    //- Global number of faces
    label nFaces_;
    // If operating on mesh faces (faceZone, patch)
      //- Local list of face IDs
      labelList faceId_;
      //- Local list of patch ID per face
      labelList facePatchId_;
      //- List of +1/-1 representing face flip map
      //  (1 use as is, -1 negate)
      labelList faceSign_;
    // If operating on sampledSurface
      //- Underlying sampledSurface
      autoPtr<sampledSurface> surfacePtr_;
  // Protected Member Functions
    //- Initialise, e.g. face addressing
    void initialise(const dictionary& dict);
    //- Return true if the field name is valid
    template<class Type>
    bool validField(const word& fieldName) const;
    //- Return field values by looking up field name
    template<class Type>
    tmp<Field<Type> > getFieldValues
    (
      const word& fieldName,
      const bool mustGet = false,
      const bool applyOrientation = false
    ) const;
    //- Apply the 'operation' to the values. Operation has to
    //  preserve Type.
    template<class Type>
    Type processSameTypeValues
    (
      const Field<Type>& values,
      const vectorField& Sf,
      const scalarField& weightField
    ) const;
    //- Apply the 'operation' to the values. Wrapper around
    //  processSameTypeValues. See also template specialisation below.
    template<class Type>
    Type processValues
    (
      const Field<Type>& values,
      const vectorField& Sf,
      const scalarField& weightField
    ) const;
    //- Output file header information
    virtual void writeFileHeader(const label i);
public:
  //- Run-time type information
  TYPE_NAME("faceSource");
  //- Construct from components
  faceSource
  (
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles = false
  );
  //- Destructor
  virtual ~faceSource();
  // Public Member Functions
    // Access
      //- Return the source type
      inline const sourceType& source() const;
      //- Return the local list of face IDs
      inline const labelList& faceId() const;
      //- Return the local list of patch ID per face
      inline const labelList& facePatch() const;
      //- Return the list of +1/-1 representing face flip map
      inline const labelList& faceSign() const;
    // Function object functions
      //- Read from dictionary
      virtual void read(const dictionary&);
      //- Calculate and write
      virtual void write();
      //- Templated helper function to output field values
      template<class Type>
      bool writeValues
      (
        const word& fieldName,
        const scalarField& weightField,
        const bool orient
      );
      //- Filter a surface field according to faceIds
      template<class Type>
      tmp<Field<Type> > filterField
      (
        const GeometricField<Type, fvsPatchField, surfaceMesh>& field,
        const bool applyOrientation
      ) const;
      //- Filter a volume field according to faceIds
      template<class Type>
      tmp<Field<Type> > filterField
      (
        const GeometricField<Type, fvPatchField, volMesh>& field,
        const bool applyOrientation
      ) const;
};
//- Specialisation of processing scalars
template<>
scalar faceSource::processValues
(
  const Field<scalar>& values,
  const vectorField& Sf,
  const scalarField& weightField
) const;
//- Specialisation of processing vectors
template<>
vector faceSource::processValues
(
  const Field<vector>& values,
  const vectorField& Sf,
  const scalarField& weightField
) const;
}  // namespace fieldValues
}  // namespace mousse

// Member Functions 
inline const mousse::fieldValues::faceSource::sourceType&
mousse::fieldValues::faceSource::source() const
{
  return source_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::faceId() const
{
  return faceId_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::facePatch() const
{
  return facePatchId_;
}
inline const mousse::labelList&
mousse::fieldValues::faceSource::faceSign() const
{
  return faceSign_;
}
#ifdef NoRepository
  #include "face_source_templates.cpp"
#endif
#endif
