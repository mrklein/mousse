// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightSurfaceWriter
// Description
//   A surfaceWriter for Ensight format.
// SourceFiles
//   ensight_surface_writer.cpp
#ifndef ensight_surface_writer_hpp_
#define ensight_surface_writer_hpp_
#include "surface_writer.hpp"
namespace mousse
{
class ensightSurfaceWriter
:
  public surfaceWriter
{
  // Private data
    //- Write option (default is IOstream::ASCII
    IOstream::streamFormat writeFormat_;
  // Private Member Functions
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
  TypeName("ensight");
  // Constructors
    //- Construct null
    ensightSurfaceWriter();
    //- Construct with some output options
    ensightSurfaceWriter(const dictionary& options);
  //- Destructor
  virtual ~ensightSurfaceWriter();
  // Member Functions
    //- True if the surface format supports geometry in a separate file.
    //  False if geometry and field must be in a single file
    virtual bool separateGeometry()
    {
      return true;
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
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<scalar>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write vectorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<vector>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write sphericalTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<sphericalTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write symmTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<symmTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write tensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<tensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
};
}  // namespace mousse
#endif
