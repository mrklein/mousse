#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CFC_CELL_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_CELL_CFC_CELL_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CFCCellToCellStencil
// Description
// SourceFiles
//   cfc_cell_to_cell_stencil.cpp
#include "cell_to_cell_stencil.hpp"
namespace mousse
{
class CFCCellToCellStencil
:
  public cellToCellStencil
{
  // Private Member Functions
    void calcFaceBoundaryData(labelList& neiGlobal) const;
    void calcCellStencil(labelListList& globalCellCells) const;
public:
  // Constructors
    //- Construct from mesh
    explicit CFCCellToCellStencil(const polyMesh& mesh);
};
}  // namespace mousse
#endif
