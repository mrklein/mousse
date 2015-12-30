// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CFCCellToFaceStencil
// Description
//   Combined corresponding cellToCellStencil of owner and neighbour.
// SourceFiles
//   cfc_cell_to_face_stencil.cpp
#ifndef cfc_cell_to_face_stencil_hpp_
#define cfc_cell_to_face_stencil_hpp_
#include "cell_to_face_stencil.hpp"
namespace mousse
{
class CFCCellToFaceStencil
:
  public cellToFaceStencil
{
public:
  // Constructors
    //- Construct from mesh
    explicit CFCCellToFaceStencil(const polyMesh&);
};
}  // namespace mousse
#endif
