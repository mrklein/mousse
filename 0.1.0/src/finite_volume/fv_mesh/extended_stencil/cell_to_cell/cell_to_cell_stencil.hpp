// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellToCellStencil
// Description
//   baseclass for extended cell centred addressing. Contains per cell a
//   list of neighbouring cells and/or boundaryfaces in global addressing.
// SourceFiles
//   cell_to_cell_stencil.cpp
#ifndef cell_to_cell_stencil_hpp_
#define cell_to_cell_stencil_hpp_
#include "global_index.hpp"
#include "bool_list.hpp"
#include "hash_set.hpp"
#include "indirect_primitive_patch.hpp"
namespace mousse
{
class polyMesh;
class cellToCellStencil
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
      void operator()(labelList& x, const labelList& y) const;
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
public:
  // Constructors
    //- Construct from mesh
    explicit cellToCellStencil(const polyMesh&);
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
