#ifndef FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_FACE_TO_CELL_CFC_FACE_TO_CELL_STENCIL_HPP_
#define FINITE_VOLUME_FV_MESH_EXTENDED_STENCIL_FACE_TO_CELL_CFC_FACE_TO_CELL_STENCIL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CFCFaceToCellStencil
// Description
// SourceFiles
//   cfc_face_to_cell_stencil.cpp
#include "face_to_cell_stencil.hpp"
namespace mousse
{
class CFCFaceToCellStencil
:
  public faceToCellStencil
{
  // Private Member Functions
    void calcFaceBoundaryData(labelListList& neiGlobal) const;
    void calcCellStencil(labelListList& globalCellFaces) const;
public:
  // Constructors
    //- Construct from mesh
    explicit CFCFaceToCellStencil(const polyMesh&);
};
}  // namespace mousse
#endif
