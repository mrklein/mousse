// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CFCFaceToCellStencil
// Description
// SourceFiles
//   cfc_face_to_cell_stencil.cpp
#ifndef cfc_face_to_cell_stencil_hpp_
#define cfc_face_to_cell_stencil_hpp_
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
