#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_INTERNAL_WRITER_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_INTERNAL_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::internalWriter
// Description
//   Write fields (internal).
// SourceFiles
//   internal_writer.cpp
//   internal_writer_templates.cpp
#include "ofstream.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "vtk_mesh.hpp"
using namespace mousse;
namespace mousse
{
class volPointInterpolation;
class internalWriter
{
  const vtkMesh& vMesh_;
  const bool binary_;
  const fileName fName_;
  std::ofstream os_;
public:
  // Constructors
    //- Construct from components
    internalWriter
    (
      const vtkMesh&,
      const bool binary,
      const fileName&
    );
  // Member Functions
    std::ofstream& os()
    {
      return os_;
    }
    //- Write cellIDs
    void writeCellIDs();
    //- Write generic GeometricFields
    template<class Type, template<class> class PatchField, class GeoMesh>
    void write
    (
      const PtrList<GeometricField<Type, PatchField, GeoMesh> >&
    );
    //- Interpolate and write volFields
    template<class Type>
    void write
    (
      const volPointInterpolation&,
      const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
    );
};
}  // namespace mousse
#ifdef NoRepository
  #include "internal_writer_templates.cpp"
#endif
#endif
