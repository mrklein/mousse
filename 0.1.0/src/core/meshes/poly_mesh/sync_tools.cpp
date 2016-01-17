// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sync_tools.hpp"
#include "global_mesh_data.hpp"

// Private Member Functions
void mousse::syncTools::swapBoundaryCellPositions
(
  const polyMesh& mesh,
  const UList<point>& cellData,
  List<point>& neighbourCellData
)
{
  if (cellData.size() != mesh.nCells())
  {
    FATAL_ERROR_IN
    (
      "syncTools<class T>::swapBoundaryCellPositions"
      "(const polyMesh&, const UList<T>&, List<T>&)"
    )   << "Number of cell values " << cellData.size()
      << " is not equal to the number of cells in the mesh "
      << mesh.nCells() << abort(FatalError);
  }
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  label nBnd = mesh.nFaces()-mesh.nInternalFaces();
  neighbourCellData.setSize(nBnd);
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    const labelUList& faceCells = pp.faceCells();
    FOR_ALL(faceCells, i)
    {
      label bFaceI = pp.start()+i-mesh.nInternalFaces();
      neighbourCellData[bFaceI] = cellData[faceCells[i]];
    }
  }
  syncTools::swapBoundaryFacePositions(mesh, neighbourCellData);
}
mousse::PackedBoolList mousse::syncTools::getMasterPoints(const polyMesh& mesh)
{
  PackedBoolList isMasterPoint(mesh.nPoints());
  PackedBoolList donePoint(mesh.nPoints());
  const globalMeshData& globalData = mesh.globalData();
  const labelList& meshPoints = globalData.coupledPatch().meshPoints();
  const labelListList& slaves = globalData.globalPointSlaves();
  const labelListList& transformedSlaves =
      globalData.globalPointTransformedSlaves();
  FOR_ALL(meshPoints, coupledPointI)
  {
    label meshPointI = meshPoints[coupledPointI];
    if
    (
      (
        slaves[coupledPointI].size()
       + transformedSlaves[coupledPointI].size()
      )
     > 0
    )
    {
      isMasterPoint[meshPointI] = true;
    }
    donePoint[meshPointI] = true;
  }
  // Do all other points
  // ~~~~~~~~~~~~~~~~~~~
  FOR_ALL(donePoint, pointI)
  {
    if (!donePoint[pointI])
    {
      isMasterPoint[pointI] = true;
    }
  }
  return isMasterPoint;
}
mousse::PackedBoolList mousse::syncTools::getMasterEdges(const polyMesh& mesh)
{
  PackedBoolList isMasterEdge(mesh.nEdges());
  PackedBoolList doneEdge(mesh.nEdges());
  const globalMeshData& globalData = mesh.globalData();
  const labelList& meshEdges = globalData.coupledPatchMeshEdges();
  const labelListList& slaves = globalData.globalEdgeSlaves();
  const labelListList& transformedSlaves =
    globalData.globalEdgeTransformedSlaves();
  FOR_ALL(meshEdges, coupledEdgeI)
  {
    label meshEdgeI = meshEdges[coupledEdgeI];
    if
    (
      (
        slaves[coupledEdgeI].size()
       + transformedSlaves[coupledEdgeI].size()
      )
     > 0
    )
    {
      isMasterEdge[meshEdgeI] = true;
    }
    doneEdge[meshEdgeI] = true;
  }
  // Do all other edges
  // ~~~~~~~~~~~~~~~~~~
  FOR_ALL(doneEdge, edgeI)
  {
    if (!doneEdge[edgeI])
    {
      isMasterEdge[edgeI] = true;
    }
  }
  return isMasterEdge;
}
mousse::PackedBoolList mousse::syncTools::getMasterFaces(const polyMesh& mesh)
{
  PackedBoolList isMasterFace(mesh.nFaces(), 1);
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    if (patches[patchI].coupled())
    {
      const coupledPolyPatch& pp =
        refCast<const coupledPolyPatch>(patches[patchI]);
      if (!pp.owner())
      {
        FOR_ALL(pp, i)
        {
          isMasterFace.unset(pp.start()+i);
        }
      }
    }
  }
  return isMasterFace;
}
mousse::PackedBoolList mousse::syncTools::getInternalOrMasterFaces
(
  const polyMesh& mesh
)
{
  PackedBoolList isMasterFace(mesh.nFaces(), 1);
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    if (pp.coupled())
    {
      if (!refCast<const coupledPolyPatch>(pp).owner())
      {
        FOR_ALL(pp, i)
        {
          isMasterFace.unset(pp.start()+i);
        }
      }
    }
    else
    {
      FOR_ALL(pp, i)
      {
        isMasterFace.unset(pp.start()+i);
      }
    }
  }
  return isMasterFace;
}
mousse::PackedBoolList mousse::syncTools::getInternalOrCoupledFaces
(
  const polyMesh& mesh
)
{
  PackedBoolList isMasterFace(mesh.nFaces(), 1);
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    if (!pp.coupled())
    {
      FOR_ALL(pp, i)
      {
        isMasterFace.unset(pp.start()+i);
      }
    }
  }
  return isMasterFace;
}
