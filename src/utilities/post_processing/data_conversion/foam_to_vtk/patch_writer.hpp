#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_PATCH_WRITER_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_PATCH_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchWriter
// Description
//   Write patch fields

#include "point_mesh.hpp"
#include "ofstream.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "vtk_mesh.hpp"
#include "indirect_primitive_patch.hpp"
#include "primitive_patch_interpolation.hpp"


using namespace mousse;


namespace mousse {

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
    std::ofstream& os() { return os_; }
    label nPoints() const { return nPoints_; }
    label nFaces() const { return nFaces_; }
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

#include "patch_writer.ipp"

#endif
