#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_SURFACE_MESH_WRITER_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_SURFACE_MESH_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceMeshWriter
// Description
//   Write faces with fields
// SourceFiles
//   surface_mesh_writer.cpp
//   surface_mesh_writer_templates.cpp
#include "point_mesh.hpp"
#include "ofstream.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "vtk_mesh.hpp"
#include "indirect_primitive_patch.hpp"
using namespace mousse;
namespace mousse
{
class volPointInterpolation;
class surfaceMeshWriter
{
  const bool binary_;
  const indirectPrimitivePatch& pp_;
  const fileName fName_;
  std::ofstream os_;
public:
  // Constructors
    //- Construct from components
    surfaceMeshWriter
    (
      const bool binary,
      const indirectPrimitivePatch& pp,
      const word& name,
      const fileName&
    );
  // Member Functions
    std::ofstream& os()
    {
      return os_;
    }
    //- Extract face data
    template<class Type>
    tmp<Field<Type> > getFaceField
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>&
    ) const;
    //- Write surfaceFields
    template<class Type>
    void write
    (
      const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    );
};
}  // namespace mousse
#ifdef NoRepository
  #include "surface_mesh_writer_templates.cpp"
#endif
#endif
