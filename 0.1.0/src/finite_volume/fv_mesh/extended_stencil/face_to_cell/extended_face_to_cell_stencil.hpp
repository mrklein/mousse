// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedFaceToCellStencil
// Description
//   Note: transformations on coupled patches not supported. Problem is the
//   positions of cells reachable through these patches.
// SourceFiles
//   extended_face_to_cell_stencil.cpp
//   extended_face_to_cell_stencil_templates.cpp
#ifndef extended_face_to_cell_stencil_hpp_
#define extended_face_to_cell_stencil_hpp_
#include "map_distribute.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class globalIndex;
class extendedFaceToCellStencil
{
protected:
  // Protected data
    const polyMesh& mesh_;
public:
  // Constructors
    //- Construct from mesh
    explicit extendedFaceToCellStencil(const polyMesh&);
  // Member Functions
    //- Use map to get the data into stencil order
    template<class T>
    static void collectData
    (
      const mapDistribute& map,
      const labelListList& stencil,
      const GeometricField<T, fvsPatchField, surfaceMesh>& fld,
      List<List<T> >& stencilFld
    );
    //- Sum surface field contributions to create cell values
    template<class Type>
    static tmp<GeometricField<Type, fvPatchField, volMesh> > weightedSum
    (
      const mapDistribute& map,
      const labelListList& stencil,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
      const List<List<scalar> >& stencilWeights
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "extended_face_to_cell_stencil_templates.cpp"
#endif
#endif
