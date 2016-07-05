#ifndef SAMPLING_SAMPLED_SURFACE_WRITERS_VTK_SURFACE_WRITER_HPP_
#define SAMPLING_SAMPLED_SURFACE_WRITERS_VTK_SURFACE_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vtkSurfaceWriter
// Description
//   A surfaceWriter for VTK legacy format.

#include "surface_writer.hpp"


namespace mousse {

class vtkSurfaceWriter
:
  public surfaceWriter
{
  // Private Member Functions
    static void writeGeometry(Ostream&, const pointField&, const faceList&);
    template<class Type>
    static void writeData(Ostream&, const Field<Type>&);
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
  TYPE_NAME("vtk");
  // Constructors
    //- Construct null
    vtkSurfaceWriter();
  //- Destructor
  virtual ~vtkSurfaceWriter();
  // Member Functions
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

