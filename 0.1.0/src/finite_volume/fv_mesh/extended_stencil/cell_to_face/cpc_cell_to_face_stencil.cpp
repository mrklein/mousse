// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cpc_cell_to_face_stencil.hpp"
#include "cpc_cell_to_cell_stencil.hpp"
// Private Member Functions 
// Constructors 
mousse::CPCCellToFaceStencil::CPCCellToFaceStencil(const polyMesh& mesh)
:
  cellToFaceStencil(mesh)
{
  // Calculate per cell the (face) connected cells (in global numbering)
  CPCCellToCellStencil globalCellCells(mesh);
  // Add stencils of neighbouring cells to create faceStencil
  labelListList faceStencil;
  calcFaceStencil(globalCellCells, faceStencil);
  // Transfer to *this
  transfer(faceStencil);
}
