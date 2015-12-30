// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchWriter
// Description
//   Write patch fields
// SourceFiles
//   patch_writer.cpp
//   patch_writer_templates.cpp
#ifndef patch_writer_hpp_
#define patch_writer_hpp_
#include "point_mesh.hpp"
#include "ofstream.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "vtk_mesh.hpp"
#include "indirect_primitive_patch.hpp"
#include "primitive_patch_interpolation.hpp"
using namespace mousse;
namespace mousse
{
class volPointInterpolation;
class patchWriter
{
  const vtkMesh& vMesh_;
  const bool binary_;
  const bool nearCellValue_;
  const fileName fName_;
  const labelList patchIDs_;
  std::ofstream os_;
  label nPoints_;
  label nFaces_;
public:
  // Constructors
    //- Construct from components
    patchWriter
    (
      const vtkMesh&,
      const bool binary,
      const bool nearCellValue,
      const fileName&,
      const labelList& patchIDs
    );
  // Member Functions
    std::ofstream& os()
    {
      return os_;
    }
    label nPoints() const
    {
      return nPoints_;
    }
    label nFaces() const
    {
      return nFaces_;
    }
    //- Write cellIDs
    void writePatchIDs();
    //- Write volFields
    template<class Type>
    void write
    (
      const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
    );
    //- Write pointFields
    template<class Type>
    void write
    (
      const PtrList<GeometricField<Type, pointPatchField, pointMesh> >&
    );
    //- Interpolate and write volFields
    template<class Type>
    void write
    (
      const PrimitivePatchInterpolation<primitivePatch>&,
      const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
    );
};
}  // namespace mousse
#ifdef NoRepository
  #include "patch_writer_templates.cpp"
#endif
#endif
