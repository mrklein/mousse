// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cfc_cell_to_cell_stencil.hpp"
#include "sync_tools.hpp"
#include "sortable_list.hpp"
#include "empty_poly_patch.hpp"
// Private Member Functions 
// Calculates per face the neighbour data (= cell or boundary face)
void mousse::CFCCellToCellStencil::calcFaceBoundaryData
(
  labelList& neiGlobal
) const
{
  const polyBoundaryMesh& patches = mesh().boundaryMesh();
  const label nBnd = mesh().nFaces()-mesh().nInternalFaces();
  const labelList& own = mesh().faceOwner();
  neiGlobal.setSize(nBnd);
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    label faceI = pp.start();
    if (pp.coupled())
    {
      // For coupled faces get the cell on the other side
      FOR_ALL(pp, i)
      {
        label bFaceI = faceI-mesh().nInternalFaces();
        neiGlobal[bFaceI] = globalNumbering().toGlobal(own[faceI]);
        faceI++;
      }
    }
    else if (isA<emptyPolyPatch>(pp))
    {
      FOR_ALL(pp, i)
      {
        label bFaceI = faceI-mesh().nInternalFaces();
        neiGlobal[bFaceI] = -1;
        faceI++;
      }
    }
    else
    {
      // For noncoupled faces get the boundary face.
      FOR_ALL(pp, i)
      {
        label bFaceI = faceI-mesh().nInternalFaces();
        neiGlobal[bFaceI] =
          globalNumbering().toGlobal(mesh().nCells()+bFaceI);
        faceI++;
      }
    }
  }
  syncTools::swapBoundaryFaceList(mesh(), neiGlobal);
}
// Calculates per cell the neighbour data (= cell or boundary in global
// numbering). First element is always cell itself!
void mousse::CFCCellToCellStencil::calcCellStencil(labelListList& globalCellCells)
const
{
  const label nBnd = mesh().nFaces()-mesh().nInternalFaces();
  const labelList& own = mesh().faceOwner();
  const labelList& nei = mesh().faceNeighbour();
  // Calculate coupled neighbour (in global numbering)
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  labelList neiGlobal(nBnd);
  calcFaceBoundaryData(neiGlobal);
  // Determine cellCells in global numbering
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  globalCellCells.setSize(mesh().nCells());
  FOR_ALL(globalCellCells, cellI)
  {
    const cell& cFaces = mesh().cells()[cellI];
    labelList& cCells = globalCellCells[cellI];
    cCells.setSize(cFaces.size()+1);
    label nNbr = 0;
    // Myself
    cCells[nNbr++] = globalNumbering().toGlobal(cellI);
    // Collect neighbouring cells/faces
    FOR_ALL(cFaces, i)
    {
      label faceI = cFaces[i];
      if (mesh().isInternalFace(faceI))
      {
        label nbrCellI = own[faceI];
        if (nbrCellI == cellI)
        {
          nbrCellI = nei[faceI];
        }
        cCells[nNbr++] = globalNumbering().toGlobal(nbrCellI);
      }
      else
      {
        label nbrCellI = neiGlobal[faceI-mesh().nInternalFaces()];
        if (nbrCellI != -1)
        {
          cCells[nNbr++] = nbrCellI;
        }
      }
    }
    cCells.setSize(nNbr);
  }
}
// Constructors 
mousse::CFCCellToCellStencil::CFCCellToCellStencil(const polyMesh& mesh)
:
  cellToCellStencil(mesh)
{
  // Calculate per cell the (face) connected cells (in global numbering)
  calcCellStencil(*this);
}
