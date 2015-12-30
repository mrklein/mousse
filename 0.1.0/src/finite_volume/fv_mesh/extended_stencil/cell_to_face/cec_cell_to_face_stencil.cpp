// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cec_cell_to_face_stencil.hpp"
#include "cec_cell_to_cell_stencil.hpp"
// Constructors 
mousse::CECCellToFaceStencil::CECCellToFaceStencil(const polyMesh& mesh)
:
  cellToFaceStencil(mesh)
{
  // Calculate per cell the (edge) connected cells (in global numbering)
  CECCellToCellStencil globalCellCells(mesh);
  // Add stencils of neighbouring cells to create faceStencil
  labelListList faceStencil;
  calcFaceStencil(globalCellCells, faceStencil);
  // Transfer to *this
  transfer(faceStencil);
}
