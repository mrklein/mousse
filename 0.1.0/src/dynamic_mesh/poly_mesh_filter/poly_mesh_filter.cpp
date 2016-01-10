// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh_filter.hpp"
#include "poly_mesh.hpp"
#include "fv_mesh.hpp"
#include "unit_conversion.hpp"
#include "edge_collapser.hpp"
#include "sync_tools.hpp"
#include "poly_topo_change.hpp"
#include "global_index.hpp"
#include "packed_bool_list.hpp"
#include "point_set.hpp"
#include "face_set.hpp"
#include "cell_set.hpp"
// Static Member Functions
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(polyMeshFilter, 0);
}
void mousse::polyMeshFilter::updateSets(const mapPolyMesh& map)
{
  updateSets<pointSet>(map);
  updateSets<faceSet>(map);
  updateSets<cellSet>(map);
}
void mousse::polyMeshFilter::copySets
(
  const polyMesh& oldMesh,
  const polyMesh& newMesh
)
{
  copySets<pointSet>(oldMesh, newMesh);
  copySets<faceSet>(oldMesh, newMesh);
  copySets<cellSet>(oldMesh, newMesh);
}
mousse::autoPtr<mousse::fvMesh> mousse::polyMeshFilter::copyMesh(const fvMesh& mesh)
{
  polyTopoChange originalMeshToNewMesh(mesh);
  autoPtr<fvMesh> meshCopy;
  autoPtr<mapPolyMesh> mapPtr = originalMeshToNewMesh.makeMesh
  (
    meshCopy,
    IOobject
    (
      mesh.name(),
      mesh.polyMesh::instance(),
      mesh.time(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    ),
    mesh,
    true // parallel sync
  );
  const mapPolyMesh& map = mapPtr();
  // Update fields
  meshCopy().updateMesh(map);
  if (map.hasMotionPoints())
  {
    meshCopy().movePoints(map.preMotionPoints());
  }
  copySets(mesh, meshCopy());
  return meshCopy;
}
// Private Member Functions 
mousse::label mousse::polyMeshFilter::filterFacesLoop(const label /*nOriginalBadFaces*/)
{
  label nBadFaces = labelMax;
  label nOuterIterations = 0;
  // Maintain the number of times a point has been part of a bad face
  labelList pointErrorCount(mesh_.nPoints(), 0);
  PackedBoolList newErrorPoint(mesh_.nPoints());
  edgeCollapser::checkMeshQuality
  (
    mesh_,
    meshQualityCoeffDict(),
    newErrorPoint
  );
  bool newBadFaces = true;
  // Main loop
  // ~~~~~~~~~
  // It tries and do some collapses, checks the resulting mesh and
  // 'freezes' some edges (by marking in minEdgeLen) and tries again.
  // This will iterate ultimately to the situation where every edge is
  // frozen and nothing gets collapsed.
  while
  (
    nOuterIterations < maxIterations()
  //&& nBadFaces > nOriginalBadFaces
  && newBadFaces
  )
  {
    Info<< nl << "Outer Iteration = " << nOuterIterations++ << nl
      << endl;
    printScalarFieldStats("Edge Filter Factor", minEdgeLen_);
    printScalarFieldStats("Face Filter Factor", faceFilterFactor_);
    // Reset the new mesh to the old mesh
    newMeshPtr_ = copyMesh(mesh_);
    fvMesh& newMesh = newMeshPtr_();
    scalarField newMeshFaceFilterFactor = faceFilterFactor_;
    pointPriority_.reset(new labelList(originalPointPriority_));
    labelList origToCurrentPointMap(identity(newMesh.nPoints()));
    {
      label nInnerIterations = 0;
      label nPrevLocalCollapse = labelMax;
      Info<< incrIndent;
      while (true)
      {
        Info<< nl << indent << "Inner iteration = "
          << nInnerIterations++ << nl << incrIndent << endl;
        label nLocalCollapse = filterFaces
        (
          newMesh,
          newMeshFaceFilterFactor,
          origToCurrentPointMap
        );
        Info<< decrIndent;
        if
        (
          nLocalCollapse >= nPrevLocalCollapse
        || nLocalCollapse == 0
        )
        {
          Info<< decrIndent;
          break;
        }
        else
        {
          nPrevLocalCollapse = nLocalCollapse;
        }
      }
    }
    scalarField newMeshMinEdgeLen = minEdgeLen_;
    {
      label nInnerIterations = 0;
      label nPrevLocalCollapse = labelMax;
      Info<< incrIndent;
      while (true)
      {
        Info<< nl << indent << "Inner iteration = "
          << nInnerIterations++ << nl << incrIndent << endl;
        label nLocalCollapse = filterEdges
        (
          newMesh,
          newMeshMinEdgeLen,
          origToCurrentPointMap
        );
        Info<< decrIndent;
        if
        (
          nLocalCollapse >= nPrevLocalCollapse
        || nLocalCollapse == 0
        )
        {
          Info<< decrIndent;
          break;
        }
        else
        {
          nPrevLocalCollapse = nLocalCollapse;
        }
      }
    }
    // Mesh check
    // ~~~~~~~~~~~~~~~~~~
    // Do not allow collapses in regions of error.
    // Updates minEdgeLen, nRelaxedEdges
    if (controlMeshQuality())
    {
      PackedBoolList isErrorPoint(newMesh.nPoints());
      nBadFaces = edgeCollapser::checkMeshQuality
      (
        newMesh,
        meshQualityCoeffDict(),
        isErrorPoint
      );
      Info<< nl << "    Number of bad faces     : " << nBadFaces << nl
        << "    Number of marked points : "
        << returnReduce(isErrorPoint.count(), sumOp<unsigned int>())
        << endl;
      updatePointErrorCount
      (
        isErrorPoint,
        origToCurrentPointMap,
        pointErrorCount
      );
      checkMeshEdgesAndRelaxEdges
      (
        newMesh,
        origToCurrentPointMap,
        isErrorPoint,
        pointErrorCount
      );
      checkMeshFacesAndRelaxEdges
      (
        newMesh,
        origToCurrentPointMap,
        isErrorPoint,
        pointErrorCount
      );
      newBadFaces = false;
      FOR_ALL(mesh_.points(), pI)
      {
        if
        (
          origToCurrentPointMap[pI] >= 0
        && isErrorPoint[origToCurrentPointMap[pI]]
        )
        {
          if (!newErrorPoint[pI])
          {
            newBadFaces = true;
            break;
          }
        }
      }
      reduce(newBadFaces, orOp<bool>());
    }
    else
    {
      return -1;
    }
  }
  return nBadFaces;
}
mousse::label mousse::polyMeshFilter::filterFaces
(
  polyMesh& newMesh,
  scalarField& newMeshFaceFilterFactor,
  labelList& origToCurrentPointMap
)
{
  // Per edge collapse status
  PackedBoolList collapseEdge(newMesh.nEdges());
  Map<point> collapsePointToLocation(newMesh.nPoints());
  edgeCollapser collapser(newMesh, collapseFacesCoeffDict());
  {
    // Collapse faces
    labelPair nCollapsedPtEdge = collapser.markSmallSliverFaces
    (
      newMeshFaceFilterFactor,
      pointPriority_(),
      collapseEdge,
      collapsePointToLocation
    );
    label nCollapsed = 0;
    FOR_ALL(nCollapsedPtEdge, collapseTypeI)
    {
      nCollapsed += nCollapsedPtEdge[collapseTypeI];
    }
    reduce(nCollapsed, sumOp<label>());
    label nToPoint = returnReduce(nCollapsedPtEdge.first(), sumOp<label>());
    label nToEdge = returnReduce(nCollapsedPtEdge.second(), sumOp<label>());
    Info<< indent
      << "Collapsing " << nCollapsed << " faces "
      << "(to point = " << nToPoint << ", to edge = " << nToEdge << ")"
      << endl;
    if (nCollapsed == 0)
    {
      return 0;
    }
  }
  // Merge edge collapses into consistent collapse-network.
  // Make sure no cells get collapsed.
  List<pointEdgeCollapse> allPointInfo;
  const globalIndex globalPoints(newMesh.nPoints());
  collapser.consistentCollapse
  (
    globalPoints,
    pointPriority_(),
    collapsePointToLocation,
    collapseEdge,
    allPointInfo
  );
  label nLocalCollapse = collapseEdge.count();
  reduce(nLocalCollapse, sumOp<label>());
  Info<< nl << indent << "Collapsing " << nLocalCollapse
    << " edges after synchronisation and PointEdgeWave" << endl;
  if (nLocalCollapse == 0)
  {
    return 0;
  }
  {
    // Apply collapses to current mesh
    polyTopoChange newMeshMod(newMesh);
    // Insert mesh refinement into polyTopoChange.
    collapser.setRefinement(allPointInfo, newMeshMod);
    Info<< indent << "Apply changes to the current mesh" << endl;
    // Apply changes to current mesh
    autoPtr<mapPolyMesh> newMapPtr = newMeshMod.changeMesh
    (
      newMesh,
      false
    );
    const mapPolyMesh& newMap = newMapPtr();
    // Update fields
    newMesh.updateMesh(newMap);
    if (newMap.hasMotionPoints())
    {
      newMesh.movePoints(newMap.preMotionPoints());
    }
    updateSets(newMap);
    updatePointPriorities(newMesh, newMap.pointMap());
    mapOldMeshFaceFieldToNewMesh
    (
      newMesh,
      newMap.faceMap(),
      newMeshFaceFilterFactor
    );
    updateOldToNewPointMap
    (
      newMap.reversePointMap(),
      origToCurrentPointMap
    );
  }
  return nLocalCollapse;
}
mousse::label mousse::polyMeshFilter::filterEdges
(
  polyMesh& newMesh,
  scalarField& newMeshMinEdgeLen,
  labelList& origToCurrentPointMap
)
{
  // Per edge collapse status
  PackedBoolList collapseEdge(newMesh.nEdges());
  Map<point> collapsePointToLocation(newMesh.nPoints());
  edgeCollapser collapser(newMesh, collapseFacesCoeffDict());
  // Work out which edges to collapse
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // This is by looking at minEdgeLen (to avoid frozen edges)
  // and marking in collapseEdge.
  label nSmallCollapsed = collapser.markSmallEdges
  (
    newMeshMinEdgeLen,
    pointPriority_(),
    collapseEdge,
    collapsePointToLocation
  );
  reduce(nSmallCollapsed, sumOp<label>());
  Info<< indent << "Collapsing " << nSmallCollapsed
    << " small edges" << endl;
  // Merge inline edges
  label nMerged = collapser.markMergeEdges
  (
    maxCos(),
    pointPriority_(),
    collapseEdge,
    collapsePointToLocation
  );
  reduce(nMerged, sumOp<label>());
  Info<< indent << "Collapsing " << nMerged << " in line edges"
    << endl;
  if (nMerged + nSmallCollapsed == 0)
  {
    return 0;
  }
  // Merge edge collapses into consistent collapse-network.
  // Make sure no cells get collapsed.
  List<pointEdgeCollapse> allPointInfo;
  const globalIndex globalPoints(newMesh.nPoints());
  collapser.consistentCollapse
  (
    globalPoints,
    pointPriority_(),
    collapsePointToLocation,
    collapseEdge,
    allPointInfo
  );
  label nLocalCollapse = collapseEdge.count();
  reduce(nLocalCollapse, sumOp<label>());
  Info<< nl << indent << "Collapsing " << nLocalCollapse
    << " edges after synchronisation and PointEdgeWave" << endl;
  if (nLocalCollapse == 0)
  {
    return 0;
  }
  // Apply collapses to current mesh
  polyTopoChange newMeshMod(newMesh);
  // Insert mesh refinement into polyTopoChange.
  collapser.setRefinement(allPointInfo, newMeshMod);
  Info<< indent << "Apply changes to the current mesh" << endl;
  // Apply changes to current mesh
  autoPtr<mapPolyMesh> newMapPtr = newMeshMod.changeMesh
  (
    newMesh,
    false
  );
  const mapPolyMesh& newMap = newMapPtr();
  // Update fields
  newMesh.updateMesh(newMap);
  if (newMap.hasMotionPoints())
  {
    newMesh.movePoints(newMap.preMotionPoints());
  }
  updateSets(newMap);
  // Synchronise the factors
  mapOldMeshEdgeFieldToNewMesh
  (
    newMesh,
    newMap.pointMap(),
    newMeshMinEdgeLen
  );
  updateOldToNewPointMap
  (
    newMap.reversePointMap(),
    origToCurrentPointMap
  );
  updatePointPriorities(newMesh, newMap.pointMap());
  return nLocalCollapse;
}
void mousse::polyMeshFilter::updatePointErrorCount
(
  const PackedBoolList& isErrorPoint,
  const labelList& oldToNewMesh,
  labelList& pointErrorCount
) const
{
  FOR_ALL(mesh_.points(), pI)
  {
    if (isErrorPoint[oldToNewMesh[pI]])
    {
      pointErrorCount[pI]++;
    }
  }
}
void mousse::polyMeshFilter::checkMeshEdgesAndRelaxEdges
(
  const polyMesh& /*newMesh*/,
  const labelList& oldToNewMesh,
  const PackedBoolList& isErrorPoint,
  const labelList& pointErrorCount
)
{
  const edgeList& edges = mesh_.edges();
  FOR_ALL(edges, edgeI)
  {
    const edge& e = edges[edgeI];
    label newStart = oldToNewMesh[e[0]];
    label newEnd = oldToNewMesh[e[1]];
    if
    (
      pointErrorCount[e[0]] >= maxPointErrorCount()
    || pointErrorCount[e[1]] >= maxPointErrorCount()
    )
    {
      minEdgeLen_[edgeI] = -1;
    }
    if
    (
      (newStart >= 0 && isErrorPoint[newStart])
    || (newEnd >= 0 && isErrorPoint[newEnd])
    )
    {
      minEdgeLen_[edgeI] *= edgeReductionFactor();
    }
  }
  syncTools::syncEdgeList(mesh_, minEdgeLen_, minEqOp<scalar>(), scalar(0.0));
  for (label smoothIter = 0; smoothIter < maxSmoothIters(); ++smoothIter)
  {
    // Smooth minEdgeLen
    FOR_ALL(mesh_.edges(), edgeI)
    {
      const edge& e = mesh_.edges()[edgeI];
      scalar sumMinEdgeLen = 0;
      label nEdges = 0;
      FOR_ALL(e, pointI)
      {
        const labelList& pEdges = mesh_.pointEdges()[e[pointI]];
        FOR_ALL(pEdges, pEdgeI)
        {
          const label pEdge = pEdges[pEdgeI];
          sumMinEdgeLen += minEdgeLen_[pEdge];
          nEdges++;
        }
      }
      minEdgeLen_[edgeI] = min
      (
        minEdgeLen_[edgeI],
        sumMinEdgeLen/nEdges
      );
    }
    syncTools::syncEdgeList
    (
      mesh_,
      minEdgeLen_,
      minEqOp<scalar>(),
      scalar(0.0)
    );
  }
}
void mousse::polyMeshFilter::checkMeshFacesAndRelaxEdges
(
  const polyMesh& /*newMesh*/,
  const labelList& oldToNewMesh,
  const PackedBoolList& isErrorPoint,
  const labelList& pointErrorCount
)
{
  const faceList& faces = mesh_.faces();
  FOR_ALL(faces, faceI)
  {
    const face& f = faces[faceI];
    FOR_ALL(f, fpI)
    {
      const label ptIndex = oldToNewMesh[f[fpI]];
      if (pointErrorCount[f[fpI]] >= maxPointErrorCount())
      {
        faceFilterFactor_[faceI] = -1;
      }
      if (isErrorPoint[ptIndex])
      {
        faceFilterFactor_[faceI] *= faceReductionFactor();
        break;
      }
    }
  }
  syncTools::syncFaceList(mesh_, faceFilterFactor_, minEqOp<scalar>());
  for (label smoothIter = 0; smoothIter < maxSmoothIters(); ++smoothIter)
  {
    // Smooth faceFilterFactor
    FOR_ALL(faces, faceI)
    {
      const labelList& fEdges = mesh_.faceEdges()[faceI];
      scalar sumFaceFilterFactors = 0;
      label nFaces = 0;
      // This is important: Only smooth around faces that share an
      // edge with a bad face
      bool skipFace = true;
      FOR_ALL(fEdges, fEdgeI)
      {
        const labelList& eFaces = mesh_.edgeFaces()[fEdges[fEdgeI]];
        FOR_ALL(eFaces, eFaceI)
        {
          const label eFace = eFaces[eFaceI];
          const face& f = faces[eFace];
          FOR_ALL(f, fpI)
          {
            const label ptIndex = oldToNewMesh[f[fpI]];
            if (isErrorPoint[ptIndex])
            {
              skipFace = false;
              break;
            }
          }
          if (eFace != faceI)
          {
            sumFaceFilterFactors += faceFilterFactor_[eFace];
            nFaces++;
          }
        }
      }
      if (skipFace)
      {
        continue;
      }
      faceFilterFactor_[faceI] = min
      (
        faceFilterFactor_[faceI],
        sumFaceFilterFactors/nFaces
      );
    }
    // Face filter factor needs to be synchronised!
    syncTools::syncFaceList(mesh_, faceFilterFactor_, minEqOp<scalar>());
  }
}
void mousse::polyMeshFilter::updatePointPriorities
(
  const polyMesh& newMesh,
  const labelList& pointMap
)
{
  labelList newPointPriority(newMesh.nPoints(), labelMin);
  const labelList& currPointPriority = pointPriority_();
  FOR_ALL(newPointPriority, ptI)
  {
    const label newPointToOldPoint = pointMap[ptI];
    const label origPointPriority = currPointPriority[newPointToOldPoint];
    newPointPriority[ptI] = max(origPointPriority, newPointPriority[ptI]);
  }
  syncTools::syncPointList
  (
    newMesh,
    newPointPriority,
    maxEqOp<label>(),
    labelMin
  );
  pointPriority_.reset(new labelList(newPointPriority));
}
void mousse::polyMeshFilter::printScalarFieldStats
(
  const string desc,
  const scalarField& fld
) const
{
  scalar sum = 0;
  scalar validElements = 0;
  scalar min = GREAT;
  scalar max = -GREAT;
  FOR_ALL(fld, i)
  {
    const scalar fldElement = fld[i];
    if (fldElement >= 0)
    {
      sum += fldElement;
      if (fldElement < min)
      {
        min = fldElement;
      }
      if (fldElement > max)
      {
        max = fldElement;
      }
      validElements++;
    }
  }
  reduce(sum, sumOp<scalar>());
  reduce(min, minOp<scalar>());
  reduce(max, maxOp<scalar>());
  reduce(validElements, sumOp<label>());
  const label totFieldSize = returnReduce(fld.size(), sumOp<label>());
  Info<< incrIndent << indent << desc
    << ": min = " << min
    << " av = " << sum/(validElements + SMALL)
    << " max = " << max << nl
    << indent
    << "    " << validElements << " / " << totFieldSize << " elements used"
    << decrIndent << endl;
}
void mousse::polyMeshFilter::mapOldMeshEdgeFieldToNewMesh
(
  const polyMesh& newMesh,
  const labelList& pointMap,
  scalarField& newMeshMinEdgeLen
) const
{
  scalarField tmp(newMesh.nEdges());
  const edgeList& newEdges = newMesh.edges();
  FOR_ALL(newEdges, newEdgeI)
  {
    const edge& newEdge = newEdges[newEdgeI];
    const label pStart = newEdge.start();
    const label pEnd = newEdge.end();
    tmp[newEdgeI] = min
    (
      newMeshMinEdgeLen[pointMap[pStart]],
      newMeshMinEdgeLen[pointMap[pEnd]]
    );
  }
  newMeshMinEdgeLen.transfer(tmp);
  syncTools::syncEdgeList
  (
    newMesh,
    newMeshMinEdgeLen,
    maxEqOp<scalar>(),
    scalar(0.0)
  );
}
void mousse::polyMeshFilter::mapOldMeshFaceFieldToNewMesh
(
  const polyMesh& newMesh,
  const labelList& faceMap,
  scalarField& newMeshFaceFilterFactor
) const
{
  scalarField tmp(newMesh.nFaces());
  FOR_ALL(faceMap, newFaceI)
  {
    const label oldFaceI = faceMap[newFaceI];
    tmp[newFaceI] = newMeshFaceFilterFactor[oldFaceI];
  }
  newMeshFaceFilterFactor.transfer(tmp);
  syncTools::syncFaceList
  (
    newMesh,
    newMeshFaceFilterFactor,
    maxEqOp<scalar>()
  );
}
void mousse::polyMeshFilter::updateOldToNewPointMap
(
  const labelList& currToNew,
  labelList& origToCurrentPointMap
) const
{
  FOR_ALL(origToCurrentPointMap, origPointI)
  {
    label oldPointI = origToCurrentPointMap[origPointI];
    if (oldPointI != -1)
    {
      label newPointI = currToNew[oldPointI];
      if (newPointI >= 0)
      {
        origToCurrentPointMap[origPointI] = newPointI;
      }
      else if (newPointI == -1)
      {
        origToCurrentPointMap[origPointI] = -1;
      }
      else
      {
        origToCurrentPointMap[origPointI] = -newPointI-2;
      }
    }
  }
}
// Constructors 
mousse::polyMeshFilter::polyMeshFilter(const fvMesh& mesh)
:
  polyMeshFilterSettings
  (
    IOdictionary
    (
      IOobject
      (
        "collapseDict",
        mesh.time().system(),
        mesh.time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      )
    )
  ),
  mesh_(mesh),
  newMeshPtr_(),
  originalPointPriority_(mesh.nPoints(), labelMin),
  pointPriority_(),
  minEdgeLen_(),
  faceFilterFactor_()
{
  writeSettings(Info);
}
mousse::polyMeshFilter::polyMeshFilter
(
  const fvMesh& mesh,
  const labelList& pointPriority
)
:
  polyMeshFilterSettings
  (
    IOdictionary
    (
      IOobject
      (
        "collapseDict",
        mesh.time().system(),
        mesh.time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      )
    )
  ),
  mesh_(mesh),
  newMeshPtr_(),
  originalPointPriority_(pointPriority),
  pointPriority_(),
  minEdgeLen_(),
  faceFilterFactor_()
{
  writeSettings(Info);
}
// Destructor 
mousse::polyMeshFilter::~polyMeshFilter()
{}
// Member Functions 
mousse::label mousse::polyMeshFilter::filter(const label nOriginalBadFaces)
{
  minEdgeLen_.resize(mesh_.nEdges(), minLen());
  faceFilterFactor_.resize(mesh_.nFaces(), initialFaceLengthFactor());
  return filterFacesLoop(nOriginalBadFaces);
}
mousse::label mousse::polyMeshFilter::filter(const faceSet& fSet)
{
  minEdgeLen_.resize(mesh_.nEdges(), minLen());
  faceFilterFactor_.resize(mesh_.nFaces(), initialFaceLengthFactor());
  FOR_ALL(faceFilterFactor_, fI)
  {
    if (!fSet.found(fI))
    {
      faceFilterFactor_[fI] = -1;
    }
  }
  return filterFacesLoop(0);
}
mousse::label mousse::polyMeshFilter::filterEdges
(
  const label nOriginalBadFaces
)
{
  label nBadFaces = labelMax/2;
  label nPreviousBadFaces = labelMax;
  label nOuterIterations = 0;
  minEdgeLen_.resize(mesh_.nEdges(), minLen());
  faceFilterFactor_.resize(0);
  labelList pointErrorCount(mesh_.nPoints(), 0);
  // Main loop
  // ~~~~~~~~~
  // It tries and do some collapses, checks the resulting mesh and
  // 'freezes' some edges (by marking in minEdgeLen) and tries again.
  // This will iterate ultimately to the situation where every edge is
  // frozen and nothing gets collapsed.
  while
  (
    nOuterIterations < maxIterations()
  && nBadFaces > nOriginalBadFaces
  && nBadFaces < nPreviousBadFaces
  )
  {
    Info<< nl << "Outer Iteration = " << nOuterIterations++ << nl
      << endl;
    printScalarFieldStats("Edge Filter Factor", minEdgeLen_);
    nPreviousBadFaces = nBadFaces;
    // Reset the new mesh to the old mesh
    newMeshPtr_ = copyMesh(mesh_);
    fvMesh& newMesh = newMeshPtr_();
    scalarField newMeshMinEdgeLen = minEdgeLen_;
    pointPriority_.reset(new labelList(originalPointPriority_));
    labelList origToCurrentPointMap(identity(newMesh.nPoints()));
    Info<< incrIndent;
    label nInnerIterations = 0;
    label nPrevLocalCollapse = labelMax;
    while (true)
    {
      Info<< nl
        << indent << "Inner iteration = " << nInnerIterations++ << nl
        << incrIndent << endl;
      label nLocalCollapse = filterEdges
      (
        newMesh,
        newMeshMinEdgeLen,
        origToCurrentPointMap
      );
      Info<< decrIndent;
      if
      (
        nLocalCollapse >= nPrevLocalCollapse
      || nLocalCollapse == 0
      )
      {
        Info<< decrIndent;
        break;
      }
      else
      {
        nPrevLocalCollapse = nLocalCollapse;
      }
    }
    // Mesh check
    // ~~~~~~~~~~~~~~~~~~
    // Do not allow collapses in regions of error.
    // Updates minEdgeLen, nRelaxedEdges
    if (controlMeshQuality())
    {
      PackedBoolList isErrorPoint(newMesh.nPoints());
      nBadFaces = edgeCollapser::checkMeshQuality
      (
        newMesh,
        meshQualityCoeffDict(),
        isErrorPoint
      );
      Info<< nl << "    Number of bad faces     : " << nBadFaces << nl
        << "    Number of marked points : "
        << returnReduce(isErrorPoint.count(), sumOp<unsigned int>())
        << endl;
      updatePointErrorCount
      (
        isErrorPoint,
        origToCurrentPointMap,
        pointErrorCount
      );
      checkMeshEdgesAndRelaxEdges
      (
        newMesh,
        origToCurrentPointMap,
        isErrorPoint,
        pointErrorCount
      );
    }
    else
    {
      return -1;
    }
  }
  return nBadFaces;
}
const mousse::autoPtr<mousse::fvMesh>& mousse::polyMeshFilter::filteredMesh() const
{
  return newMeshPtr_;
}
const mousse::autoPtr<mousse::labelList>&
mousse::polyMeshFilter::pointPriority() const
{
  return pointPriority_;
}
