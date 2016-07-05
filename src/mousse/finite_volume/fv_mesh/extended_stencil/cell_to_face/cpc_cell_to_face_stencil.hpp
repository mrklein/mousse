#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CPC_CELL_TO_FACE_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_CELL_TO_FACE_CPC_CELL_TO_FACE_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CPCCellToFaceStencil
// Description
//   Combined corresponding cellToCellStencil of owner and neighbour.
// SourceFiles
//   cpc_cell_to_face_stencil.cpp
#include "cell_to_face_stencil.hpp"
namespace mousse
{
class CPCCellToFaceStencil
:
  public cellToFaceStencil
{
public:
  // Constructors
    //- Construct from all cells and boundary faces
    explicit CPCCellToFaceStencil(const polyMesh&);
};
}  // namespace mousse
#endif
