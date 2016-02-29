#ifndef SAMPLING_SAMPLED_SURFACE_WRITERS_NASTRAN_SURFACE_WRITER_HPP_
#define SAMPLING_SAMPLED_SURFACE_WRITERS_NASTRAN_SURFACE_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nastranSurfaceWriter
// Description
//   A surface writer for the Nastran file format - both surface mesh and fields
//   formatOptions
//   {
//     nastran
//     {
//       // From OpenFOAM field name to Nastran field name
//       fields ((pMean PLOAD2));
//       // Optional scale
//       scale 2.0;
//       // Optional format
//       format free;    //short, long, free
//     }
//   };
// SourceFiles
//   nastran_surface_writer.cpp
//   nastran_surface_writer_templates.cpp
#include "surface_writer.hpp"
#include "named_enum.hpp"
#include "ofstream.hpp"
namespace mousse
{
class nastranSurfaceWriter
:
  public surfaceWriter
{
public:
  enum writeFormat
  {
    wfShort,
    wfLong,
    wfFree
  };
  static const NamedEnum<writeFormat, 3> writeFormatNames_;
private:
  // Private data
    //- Write option
    writeFormat writeFormat_;
    //- Map of OpenFOAM field name vs nastran field name
    HashTable<word> fieldMap_;
    //- Scale to apply to values (default = 1.0)
    scalar scale_;
  // Private Member Functions
    //- Initialise the output stream format params
    void formatOS(OFstream& os) const;
    //- Write a co-ordinate
    void writeCoord
    (
      const point& p,
      const label pointI,
      OFstream& os
    ) const;
    //- Write a face element (CTRIA3 or CQUAD4)
    void writeFace
    (
      const word& faceType,
      const labelList& facePts,
      label& nFace,
      OFstream& os
    ) const;
    //- Main driver to write the surface mesh geometry
    void writeGeometry
    (
      const pointField& points,
      const faceList& faces,
      List<DynamicList<face> >& decomposedFaces,
      OFstream& os
    ) const;
    //- Write a face-based value
    template<class Type>
    void writeFaceValue
    (
      const word& nasFieldName,
      const Type& value,
      const label EID,
      OFstream& os
    ) const;
    //- Templated write operation
    template<class Type>
    void writeTemplate
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<Type>& values,
      const bool isNodeValues,
      const bool verbose
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("nastran");
  // Constructors
    //- Construct null
    nastranSurfaceWriter();
    //- Construct with some output options
    nastranSurfaceWriter(const dictionary& options);
  //- Destructor
  virtual ~nastranSurfaceWriter();
  // Member Functions
    //- True if the surface format supports geometry in a separate file.
    //  False if geometry and field must be in a single file
    virtual bool separateGeometry()
    {
      return false;
    }
    //- Write single surface geometry to file.
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const bool verbose = false
    ) const;
    //- Write scalarField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<scalar>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write vectorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<vector>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write sphericalTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<sphericalTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write symmTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<symmTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write tensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<tensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "nastran_surface_writer_templates.cpp"
#endif
#endif
