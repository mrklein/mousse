#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_FUNS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_FUNS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::writeFuns
// Description
//   Various functions for collecting and writing binary data.
// SourceFiles
//   write_funs.cpp
#include "float_scalar.hpp"
#include "dynamic_list.hpp"
#include "vol_fields_fwd.hpp"
#include "point_fields_fwd.hpp"
#include "vtk_mesh.hpp"
#include "vol_point_interpolation.hpp"
namespace mousse
{
class writeFuns
{
  // Private Member Functions
    // Swap halves of word.
      static void swapWord(label& word32);
      static void swapWords(const label nWords, label* words32);
public:
  // Write ascii or binary. If binary optionally in-place swaps argument
    static void write(std::ostream&, const bool, List<floatScalar>&);
    static void write(std::ostream&, const bool, DynamicList<floatScalar>&);
    static void write(std::ostream&, const bool, labelList&);
    static void write(std::ostream&, const bool, DynamicList<label>&);
  // Write header
    static void writeHeader
    (
      std::ostream&,
      const bool isBinary,
      const std::string& title
    );
    static void writeCellDataHeader
    (
      std::ostream&,
      const label nCells,
      const label nFields
    );
    static void writePointDataHeader
    (
      std::ostream&,
      const label nPoints,
      const label nFields
    );
  // Convert to VTK and store
    static void insert(const scalar, DynamicList<floatScalar>&);
    static void insert(const point&, DynamicList<floatScalar>&);
    static void insert(const sphericalTensor&, DynamicList<floatScalar>&);
    static void insert(const symmTensor&, DynamicList<floatScalar>&);
    static void insert(const tensor&, DynamicList<floatScalar>&);
  //- Append elements to DynamicList
  static void insert(const labelList&, DynamicList<label>&);
  template<class Type>
  static void insert(const List<Type>&, DynamicList<floatScalar>&);
  //- Write volField with cell values (including decomposed cells)
  template<class Type>
  static void write
  (
    std::ostream&,
    const bool binary,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const vtkMesh&
  );
  //- Write pointField on all mesh points. Interpolate to cell centre
  //  for decomposed cell centres.
  template<class Type>
  static void write
  (
    std::ostream&,
    const bool binary,
    const GeometricField<Type, pointPatchField, pointMesh>&,
    const vtkMesh&
  );
  //- Write interpolated field on points and original cell values on
  //  decomposed cell centres.
  template<class Type>
  static void write
  (
    std::ostream&,
    const bool binary,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const GeometricField<Type, pointPatchField, pointMesh>&,
    const vtkMesh&
  );
  //- Write generic GeometricFields
  template<class Type, template<class> class PatchField, class GeoMesh>
  static void write
  (
    std::ostream&,
    const bool binary,
    const PtrList<GeometricField<Type, PatchField, GeoMesh> >&,
    const vtkMesh&
  );
  //- Interpolate and write volFields
  template<class Type>
  static void write
  (
    std::ostream&,
    const bool binary,
    const volPointInterpolation&,
    const PtrList<GeometricField<Type, fvPatchField, volMesh> >&,
    const vtkMesh&
  );
};
}  // namespace mousse
#ifdef NoRepository
  #include "write_funs_templates.cpp"
#endif
#endif
