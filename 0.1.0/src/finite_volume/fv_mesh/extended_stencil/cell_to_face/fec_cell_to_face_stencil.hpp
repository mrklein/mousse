// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FECCellToFaceStencil
// Description
//   All cells connected via edge to face.
// SourceFiles
//   fec_cell_to_face_stencil.cpp
#ifndef fec_cell_to_face_stencil_hpp_
#define fec_cell_to_face_stencil_hpp_
#include "cell_to_face_stencil.hpp"
#include "edge_map.hpp"
namespace mousse
{
class FECCellToFaceStencil
:
  public cellToFaceStencil
{
  // Private Member Functions
    //- Calculates per edge the neighbour data (= edgeCells)
    void calcEdgeBoundaryData
    (
      const boolList& isValidBFace,
      const labelList& boundaryEdges,
      EdgeMap<labelList>& neiGlobal
    ) const;
    void calcFaceStencil(labelListList& faceStencil) const;
public:
  // Constructors
    //- Construct from all cells and boundary faces
    explicit FECCellToFaceStencil(const polyMesh&);
};
}  // namespace mousse
#endif
