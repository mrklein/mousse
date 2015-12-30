// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CFCCellToCellStencil
// Description
// SourceFiles
//   cfc_cell_to_cell_stencil.cpp
#ifndef cfc_cell_to_cell_stencil_hpp_
#define cfc_cell_to_cell_stencil_hpp_
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
