#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_EXTENDED_CELL_TO_FACE_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_EXTENDED_CELL_TO_FACE_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extendedCellToFaceStencil
// Description
//   Calculates/constains the extended cell-to-face stencil.
//   The stencil is a list of indices into either cells or boundary faces
//   in a compact way. (element 0 is owner, 1 is neighbour). The index numbering
//   is
//   - cells first
//   - then all (non-empty patch) boundary faces
//   When used in evaluation is a two stage process:
//   - collect the data (cell data and non-empty boundaries) into a
//   single field
//   - (parallel) distribute the field
//   - sum the weights*field.
// SourceFiles
//   extended_cell_to_face_stencil.cpp
//   extended_cell_to_face_stencil_templates.cpp
#include "map_distribute.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class globalIndex;
class extendedCellToFaceStencil
{
protected:
  // Protected data
    const polyMesh& mesh_;
protected:
    //- Write some statistics about stencil
    static void writeStencilStats
    (
      Ostream& os,
      const labelListList& stencil,
      const mapDistribute& map
    );
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("extendedCellToFaceStencil");
  // Constructors
    //- Construct from mesh
    explicit extendedCellToFaceStencil(const polyMesh&);
  // Member Functions
    //- Use map to get the data into stencil order
    template<class T>
    static void collectData
    (
      const mapDistribute& map,
      const labelListList& stencil,
      const GeometricField<T, fvPatchField, volMesh>& fld,
      List<List<T> >& stencilFld
    );
    //- Sum vol field contributions to create face values
    template<class Type>
    static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    weightedSum
    (
      const mapDistribute& map,
      const labelListList& stencil,
      const GeometricField<Type, fvPatchField, volMesh>& fld,
      const List<List<scalar> >& stencilWeights
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "extended_cell_to_face_stencil_templates.cpp"
#endif
#endif
