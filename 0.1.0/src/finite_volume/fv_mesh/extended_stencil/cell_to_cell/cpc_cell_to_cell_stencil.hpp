// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CPCCellToCellStencil
// Description
// SourceFiles
//   cpc_cell_to_cell_stencil.cpp
#ifndef cpc_cell_to_cell_stencil_hpp_
#define cpc_cell_to_cell_stencil_hpp_
#include "cell_to_cell_stencil.hpp"
#include "bool_list.hpp"
#include "hash_set.hpp"
#include "map.hpp"
namespace mousse
{
class CPCCellToCellStencil
:
  public cellToCellStencil
{
  // Private Member Functions
    //- Calculates per point the neighbour data (= pointCells)
    void calcPointBoundaryData
    (
      const boolList& isValidBFace,
      const labelList& boundaryPoints,
      Map<labelList>& neiGlobal
    ) const;
    void calcCellStencil(labelListList& globalCellCells) const;
public:
  // Constructors
    //- Construct from all cells and boundary faces
    explicit CPCCellToCellStencil(const polyMesh&);
};
}  // namespace mousse
#endif
