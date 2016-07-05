#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CELL_TO_FACE_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CELL_TO_FACE_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellToFaceStencil
// Description
//   Base class for extended cell-to-face stencils (face values from
//   neighbouring cells)
// SourceFiles
//   face_stencil.cpp
#include "global_index.hpp"
#include "bool_list.hpp"
#include "hash_set.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
class polyMesh;
class cellToFaceStencil
:
  public labelListList
{
  // Private data
    const polyMesh& mesh_;
    //- Global numbering for cells and boundary faces
    const globalIndex globalNumbering_;
protected:
    //- Merge two lists.
    static void merge
    (
      const label,
      const label,
      const labelList&,
      labelList&
    );
    //- Merge two lists.
    static void merge(const label, const labelList&, labelList&);
    //- Valid boundary faces (not empty and not coupled)
    void validBoundaryFaces(boolList& isValidBFace) const;
    //- Return patch of all coupled faces.
    autoPtr<indirectPrimitivePatch> allCoupledFacesPatch() const;
    //- Combine operator for labelLists
    class unionEqOp
    {
      public:
      void operator()( labelList& x, const labelList& y ) const;
    };
    //- Collect cell neighbours of faces in global numbering
    void insertFaceCells
    (
      const label exclude0,
      const label exclude1,
      const boolList& nonEmptyFace,
      const labelList& faceLabels,
      labelHashSet& globals
    ) const;
    //- Collect cell neighbours of faces in global numbering
    labelList calcFaceCells
    (
      const boolList& nonEmptyFace,
      const labelList& faceLabels,
      labelHashSet& globals
    ) const;
    //- Collect cell neighbours into extended stencil
    void calcFaceStencil
    (
      const labelListList& globalCellCells,
      labelListList& faceStencil
    ) const;
public:
  // Constructors
    //- Construct from mesh
    explicit cellToFaceStencil(const polyMesh&);
  // Member Functions
    const polyMesh& mesh() const
    {
      return mesh_;
    }
    //- Global numbering for cells and boundary faces
    const globalIndex& globalNumbering() const
    {
      return globalNumbering_;
    }
};
}  // namespace mousse
#endif
