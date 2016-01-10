// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cpc_cell_to_cell_stencil.hpp"
#include "sync_tools.hpp"
#include "dummy_transform.hpp"
// Private Member Functions 
// Calculates per point the neighbour data (= pointCells)
void mousse::CPCCellToCellStencil::calcPointBoundaryData
(
  const boolList& isValidBFace,
  const labelList& boundaryPoints,
  Map<labelList>& neiGlobal
) const
{
  neiGlobal.resize(2*boundaryPoints.size());
  labelHashSet pointGlobals;
  FOR_ALL(boundaryPoints, i)
  {
    label pointI = boundaryPoints[i];
    neiGlobal.insert
    (
      pointI,
      calcFaceCells
      (
        isValidBFace,
        mesh().pointFaces()[pointI],
        pointGlobals
      )
    );
  }
  syncTools::syncPointMap
  (
    mesh(),
    neiGlobal,
    unionEqOp(),
    mousse::dummyTransform()      // dummy transformation
  );
}
// Calculates per cell the neighbour data (= cell or boundary in global
// numbering). First element is always cell itself!
void mousse::CPCCellToCellStencil::calcCellStencil
(
  labelListList& globalCellCells
) const
{
  // Calculate points on coupled patches
  labelList boundaryPoints(allCoupledFacesPatch()().meshPoints());
  // Mark boundary faces to be included in stencil (i.e. not coupled or empty)
  boolList isValidBFace;
  validBoundaryFaces(isValidBFace);
  // Swap pointCells for coupled points
  Map<labelList> neiGlobal;
  calcPointBoundaryData
  (
    isValidBFace,
    boundaryPoints,
    neiGlobal
  );
  globalCellCells.setSize(mesh().nCells());
  // Do coupled points first
  FOR_ALL(boundaryPoints, i)
  {
    label pointI = boundaryPoints[i];
    const labelList& pGlobals = neiGlobal[pointI];
    // Distribute to all pointCells
    const labelList& pCells = mesh().pointCells(pointI);
    FOR_ALL(pCells, j)
    {
      label cellI = pCells[j];
      // Insert pGlobals into globalCellCells
      merge
      (
        globalNumbering().toGlobal(cellI),
        pGlobals,
        globalCellCells[cellI]
      );
    }
  }
  neiGlobal.clear();
  // Do remaining points cells
  labelHashSet pointGlobals;
  for (label pointI = 0; pointI < mesh().nPoints(); pointI++)
  {
    labelList pGlobals
    (
      calcFaceCells
      (
        isValidBFace,
        mesh().pointFaces()[pointI],
        pointGlobals
      )
    );
    const labelList& pCells = mesh().pointCells(pointI);
    FOR_ALL(pCells, j)
    {
      label cellI = pCells[j];
      merge
      (
        globalNumbering().toGlobal(cellI),
        pGlobals,
        globalCellCells[cellI]
      );
    }
  }
}
// Constructors 
mousse::CPCCellToCellStencil::CPCCellToCellStencil(const polyMesh& mesh)
:
  cellToCellStencil(mesh)
{
  // Calculate per cell the (point) connected cells (in global numbering)
  labelListList globalCellCells;
  calcCellStencil(*this);
}
