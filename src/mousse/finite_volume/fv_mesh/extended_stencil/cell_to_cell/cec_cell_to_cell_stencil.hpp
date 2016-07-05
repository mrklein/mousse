#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CEC_CELL_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CEC_CELL_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CECCellToCellStencil
// Description
// SourceFiles
//   cec_cell_to_cell_stencil.cpp
#include "cell_to_cell_stencil.hpp"
#include "bool_list.hpp"
#include "hash_set.hpp"
#include "map.hpp"
#include "edge_map.hpp"
namespace mousse
{
class CECCellToCellStencil
:
  public cellToCellStencil
{
  // Private Member Functions
    //- Calculates per edge the neighbour data (= edgeCells)
    void calcEdgeBoundaryData
    (
      const boolList& isValidBFace,
      const labelList& boundaryEdges,
      EdgeMap<labelList>& neiGlobal
    ) const;
    void calcCellStencil(labelListList& globalCellCells) const;
public:
  // Constructors
    //- Construct from all cells and boundary faces
    explicit CECCellToCellStencil(const polyMesh&);
};
}  // namespace mousse
#endif
