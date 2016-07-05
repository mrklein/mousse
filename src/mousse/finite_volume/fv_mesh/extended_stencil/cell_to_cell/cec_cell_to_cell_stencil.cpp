// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cec_cell_to_cell_stencil.hpp"
#include "sync_tools.hpp"
#include "dummy_transform.hpp"
// Private Member Functions 
// Calculates per edge the neighbour data (= edgeCells)
void mousse::CECCellToCellStencil::calcEdgeBoundaryData
(
  const boolList& isValidBFace,
  const labelList& boundaryEdges,
  EdgeMap<labelList>& neiGlobal
) const
{
  neiGlobal.resize(2*boundaryEdges.size());
  labelHashSet edgeGlobals;
  FOR_ALL(boundaryEdges, i)
  {
    label edgeI = boundaryEdges[i];
    neiGlobal.insert
    (
      mesh().edges()[edgeI],
      calcFaceCells
      (
        isValidBFace,
        mesh().edgeFaces(edgeI),
        edgeGlobals
      )
    );
  }
  syncTools::syncEdgeMap(mesh(), neiGlobal, unionEqOp(), dummyTransform());
}
// Calculates per cell the neighbour data (= cell or boundary in global
// numbering). First element is always cell itself!
void mousse::CECCellToCellStencil::calcCellStencil
(
  labelListList& globalCellCells
) const
{
  // Calculate edges on coupled patches
  labelList boundaryEdges
  (
    allCoupledFacesPatch()().meshEdges
    (
      mesh().edges(),
      mesh().pointEdges()
    )
  );
  //{
  //    OFstream str(mesh().time().path()/"boundaryEdges.obj");
  //    Pout<< "DUmping boundary edges to " << str.name() << endl;
  //
  //    label vertI = 0;
  //    FOR_ALL(boundaryEdges, i)
  //    {
  //        label edgeI = boundaryEdges[i];
  //        const edge& e = mesh().edges()[edgeI];
  //        const point& p0 = mesh().points()[e[0]];
  //        const point& p1 = mesh().points()[e[1]];
  //
  //        Pout<< "boundary edge " << edgeI << " between " << p0 << p1
  //            << endl;
  //
  //        meshTools::writeOBJ(str, p0);
  //        vertI++;
  //        meshTools::writeOBJ(str, p1);
  //        vertI++;
  //        str << "l " << vertI-1 << ' ' << vertI << nl;
  //    }
  //}
  // Mark boundary faces to be included in stencil (i.e. not coupled or empty)
  boolList isValidBFace;
  validBoundaryFaces(isValidBFace);
  // Swap edgeCells for coupled edges. Note: use EdgeMap for now since we've
  // got syncTools::syncEdgeMap for those. Should be replaced with Map and
  // syncTools functionality to handle those.
  EdgeMap<labelList> neiGlobal;
  calcEdgeBoundaryData
  (
    isValidBFace,
    boundaryEdges,
    neiGlobal
  );
  globalCellCells.setSize(mesh().nCells());
  // Do coupled edges first
  FOR_ALL(boundaryEdges, i)
  {
    label edgeI = boundaryEdges[i];
    const labelList& eGlobals = neiGlobal[mesh().edges()[edgeI]];
    // Distribute to all edgeCells
    const labelList& eCells = mesh().edgeCells(edgeI);
    FOR_ALL(eCells, j)
    {
      label cellI = eCells[j];
      // Insert pGlobals into globalCellCells
      merge
      (
        globalNumbering().toGlobal(cellI),
        eGlobals,
        globalCellCells[cellI]
      );
    }
  }
  neiGlobal.clear();
  // Do remaining edges cells
  labelHashSet edgeGlobals;
  for (label edgeI = 0; edgeI < mesh().nEdges(); edgeI++)
  {
    labelList eGlobals
    (
      calcFaceCells
      (
        isValidBFace,
        mesh().edgeFaces(edgeI),
        edgeGlobals
      )
    );
    const labelList& eCells = mesh().edgeCells(edgeI);
    FOR_ALL(eCells, j)
    {
      label cellI = eCells[j];
      merge
      (
        globalNumbering().toGlobal(cellI),
        eGlobals,
        globalCellCells[cellI]
      );
    }
  }
}
// Constructors 
mousse::CECCellToCellStencil::CECCellToCellStencil(const polyMesh& mesh)
:
  cellToCellStencil(mesh)
{
  // Calculate per cell the (edge) connected cells (in global numbering)
  calcCellStencil(*this);
}
