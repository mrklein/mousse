// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distributed_tri_surface_mesh.hpp"
#include "map_distribute.hpp"
#include "random.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "triangle_funcs.hpp"
#include "match_points.hpp"
#include "global_index.hpp"
#include "time.hpp"
#include "ifstream.hpp"
#include "decomposition_method.hpp"
#include "geom_decomp.hpp"
#include "vector_list.hpp"
#include "packed_bool_list.hpp"
#include "patch_tools.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(distributedTriSurfaceMesh, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  searchableSurface,
  distributedTriSurfaceMesh,
  dict
);

template<>
const char* mousse::NamedEnum
<
  mousse::distributedTriSurfaceMesh::distributionType,
  3
>::names[] =
{
  "follow",
  "independent",
  "frozen"
};

}

const mousse::NamedEnum<mousse::distributedTriSurfaceMesh::distributionType, 3>
  mousse::distributedTriSurfaceMesh::distributionTypeNames_;


// Private Member Functions 

// Read my additional data from the dictionary
bool mousse::distributedTriSurfaceMesh::read()
{
  // Get bb of all domains.
  procBb_.setSize(Pstream::nProcs());
  procBb_[Pstream::myProcNo()] = List<treeBoundBox>{dict_.lookup("bounds")};
  Pstream::gatherList(procBb_);
  Pstream::scatterList(procBb_);
  // Distribution type
  distType_ = distributionTypeNames_.read(dict_.lookup("distributionType"));
  // Merge distance
  mergeDist_ = readScalar(dict_.lookup("mergeDistance"));
  return true;
}


// Is segment fully local?
bool mousse::distributedTriSurfaceMesh::isLocal
(
  const List<treeBoundBox>& myBbs,
  const point& start,
  const point& end
)
{
  FOR_ALL(myBbs, bbI) {
    if (myBbs[bbI].contains(start) && myBbs[bbI].contains(end)) {
      return true;
    }
  }
  return false;
}


void mousse::distributedTriSurfaceMesh::distributeSegment
(
  const label segmentI,
  const point& start,
  const point& end,
  DynamicList<segment>& allSegments,
  DynamicList<label>& allSegmentMap,
  List<DynamicList<label>>& sendMap
) const
{
  // 1. Fully local already handled outside. Note: retest is cheap.
  if (isLocal(procBb_[Pstream::myProcNo()], start, end)) {
    return;
  }
  // 2. If fully inside one other processor, then only need to send
  // to that one processor even if it intersects another. Rare occurrence
  // but cheap to test.
  FOR_ALL(procBb_, procI) {
    if (procI != Pstream::myProcNo()) {
      const List<treeBoundBox>& bbs = procBb_[procI];
      if (isLocal(bbs, start, end)) {
        sendMap[procI].append(allSegments.size());
        allSegmentMap.append(segmentI);
        allSegments.append(segment(start, end));
        return;
      }
    }
  }
  // 3. If not contained in single processor send to all intersecting
  // processors.
  FOR_ALL(procBb_, procI) {
    const List<treeBoundBox>& bbs = procBb_[procI];
    FOR_ALL(bbs, bbI) {
      const treeBoundBox& bb = bbs[bbI];
      // Scheme a: any processor that intersects the segment gets
      // the segment.
      // Intersection point
      point clipPt;
      if (bb.intersects(start, end, clipPt)) {
        sendMap[procI].append(allSegments.size());
        allSegmentMap.append(segmentI);
        allSegments.append(segment(start, end));
      }
    }
  }
}


mousse::autoPtr<mousse::mapDistribute>
mousse::distributedTriSurfaceMesh::distributeSegments
(
  const pointField& start,
  const pointField& end,
  List<segment>& allSegments,
  labelList& allSegmentMap
) const
{
  // Determine send map
  labelListList sendMap{Pstream::nProcs()};

  {
    // Since intersection test is quite expensive compared to memory
    // allocation we use DynamicList to immediately store the segment
    // in the correct bin.
    // Segments to test
    DynamicList<segment> dynAllSegments{start.size()};
    // Original index of segment
    DynamicList<label> dynAllSegmentMap{start.size()};
    // Per processor indices into allSegments to send
    List<DynamicList<label>> dynSendMap{Pstream::nProcs()};
    FOR_ALL(start, segmentI) {
      distributeSegment
        (
          segmentI,
          start[segmentI],
          end[segmentI],
          dynAllSegments,
          dynAllSegmentMap,
          dynSendMap
        );
    }
    // Convert dynamicList to labelList
    sendMap.setSize(Pstream::nProcs());
    FOR_ALL(sendMap, procI) {
      dynSendMap[procI].shrink();
      sendMap[procI].transfer(dynSendMap[procI]);
    }
    allSegments.transfer(dynAllSegments.shrink());
    allSegmentMap.transfer(dynAllSegmentMap.shrink());
  }
  // Send over how many I need to receive.
  labelListList sendSizes{Pstream::nProcs()};
  sendSizes[Pstream::myProcNo()].setSize(Pstream::nProcs());
  FOR_ALL(sendMap, procI) {
    sendSizes[Pstream::myProcNo()][procI] = sendMap[procI].size();
  }
  Pstream::gatherList(sendSizes);
  Pstream::scatterList(sendSizes);
  // Determine order of receiving
  labelListList constructMap{Pstream::nProcs()};
  // My local segments first
  constructMap[Pstream::myProcNo()] =
    identity(sendMap[Pstream::myProcNo()].size());
  label segmentI = constructMap[Pstream::myProcNo()].size();
  FOR_ALL(constructMap, procI) {
    if (procI != Pstream::myProcNo()) {
      // What I need to receive is what other processor is sending to me.
      label nRecv = sendSizes[procI][Pstream::myProcNo()];
      constructMap[procI].setSize(nRecv);
      for (label i = 0; i < nRecv; i++) {
        constructMap[procI][i] = segmentI++;
      }
    }
  }
  return autoPtr<mapDistribute>
  {
    new mapDistribute
    {
      segmentI,       // size after construction
      sendMap.xfer(),
      constructMap.xfer()
    }
  };
}


void mousse::distributedTriSurfaceMesh::findLine
(
  const bool nearestIntersection,
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  // Initialise
  info.setSize(start.size());
  FOR_ALL(info, i) {
    info[i].setMiss();
  }
  if (!Pstream::parRun()) {
    FOR_ALL(start, i) {
      if (nearestIntersection) {
        info[i] = octree.findLine(start[i], end[i]);
      } else {
        info[i] = octree.findLineAny(start[i], end[i]);
      }
    }
  } else {
    // Important:force synchronised construction of indexing
    const globalIndex& triIndexer = globalTris();
    // Do any local queries
    // ~~~~~~~~~~~~~~~~~~~~
    label nLocal = 0;
    FOR_ALL(start, i) {
      if (isLocal(procBb_[Pstream::myProcNo()], start[i], end[i])) {
        if (nearestIntersection) {
          info[i] = octree.findLine(start[i], end[i]);
        } else {
          info[i] = octree.findLineAny(start[i], end[i]);
        }
        if (info[i].hit()) {
          info[i].setIndex(triIndexer.toGlobal(info[i].index()));
        }
        nLocal++;
      }
    }
    const label l1 = returnReduce(nLocal, sumOp<label>());
    const label l2 = returnReduce(start.size(), sumOp<label>());
    if (l1 < l2) {
      // Not all can be resolved locally. Build segments and map,
      // send over segments, do intersections, send back and merge.
      // Construct queries (segments)
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // Segments to test
      List<segment> allSegments{start.size()};
      // Original index of segment
      labelList allSegmentMap{start.size()};
      const autoPtr<mapDistribute> mapPtr
      {
        distributeSegments
          (
            start,
            end,
            allSegments,
            allSegmentMap
          )
      };
      const mapDistribute& map = mapPtr();
      label nOldAllSegments = allSegments.size();
      // Exchange the segments
      // ~~~~~~~~~~~~~~~~~~~~~
      map.distribute(allSegments);
      // Do tests I need to do
      // ~~~~~~~~~~~~~~~~~~~~~
      // Intersections
      List<pointIndexHit> intersections{allSegments.size()};
      FOR_ALL(allSegments, i) {
        if (nearestIntersection) {
          intersections[i] =
            octree.findLine(allSegments[i].first(), allSegments[i].second());
        } else {
          intersections[i] =
            octree.findLineAny(allSegments[i].first(), allSegments[i].second());
        }
        // Convert triangle index to global numbering
        if (intersections[i].hit()) {
          intersections[i].setIndex
          (
            triIndexer.toGlobal(intersections[i].index())
          );
        }
      }
      // Exchange the intersections (opposite to segments)
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      map.reverseDistribute(nOldAllSegments, intersections);
      // Extract the hits
      // ~~~~~~~~~~~~~~~~
      FOR_ALL(intersections, i) {
        const pointIndexHit& allInfo = intersections[i];
        label segmentI = allSegmentMap[i];
        pointIndexHit& hitInfo = info[segmentI];
        if (allInfo.hit()) {
          if (!hitInfo.hit()) {
            // No intersection yet so take this one
            hitInfo = allInfo;
          } else if (nearestIntersection) {
            // Nearest intersection
            const scalar r1 = magSqr(allInfo.hitPoint() - start[segmentI]);
            const scalar r2 = magSqr(hitInfo.hitPoint() - start[segmentI]);
            if (r1 < r2) {
              hitInfo = allInfo;
            }
          }
        }
      }
    }
  }
}


// Exchanges indices to the processor they come from.
// - calculates exchange map
// - uses map to calculate local triangle index
mousse::autoPtr<mousse::mapDistribute>
mousse::distributedTriSurfaceMesh::calcLocalQueries
(
  const List<pointIndexHit>& info,
  labelList& triangleIndex
) const
{
  triangleIndex.setSize(info.size());
  const globalIndex& triIndexer = globalTris();
  // Determine send map
  // ~~~~~~~~~~~~~~~~~~
  // Since determining which processor the query should go to is
  // cheap we do a multi-pass algorithm to save some memory temporarily.
  // 1. Count
  labelList nSend{Pstream::nProcs(), 0};
  FOR_ALL(info, i) {
    if (info[i].hit()) {
      label procI = triIndexer.whichProcID(info[i].index());
      nSend[procI]++;
    }
  }
  // 2. Size sendMap
  labelListList sendMap{Pstream::nProcs()};
  FOR_ALL(nSend, procI) {
    sendMap[procI].setSize(nSend[procI]);
    nSend[procI] = 0;
  }
  // 3. Fill sendMap
  FOR_ALL(info, i) {
    if (info[i].hit()) {
      label procI = triIndexer.whichProcID(info[i].index());
      triangleIndex[i] = triIndexer.toLocal(procI, info[i].index());
      sendMap[procI][nSend[procI]++] = i;
    } else {
      triangleIndex[i] = -1;
    }
  }
  // Send over how many I need to receive
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  labelListList sendSizes{Pstream::nProcs()};
  sendSizes[Pstream::myProcNo()].setSize(Pstream::nProcs());
  FOR_ALL(sendMap, procI) {
    sendSizes[Pstream::myProcNo()][procI] = sendMap[procI].size();
  }
  Pstream::gatherList(sendSizes);
  Pstream::scatterList(sendSizes);
  // Determine receive map
  // ~~~~~~~~~~~~~~~~~~~~~
  labelListList constructMap{Pstream::nProcs()};
  // My local segments first
  constructMap[Pstream::myProcNo()] =
    identity(sendMap[Pstream::myProcNo()].size());
  label segmentI = constructMap[Pstream::myProcNo()].size();
  FOR_ALL(constructMap, procI) {
    if (procI != Pstream::myProcNo()) {
      // What I need to receive is what other processor is sending to me.
      label nRecv = sendSizes[procI][Pstream::myProcNo()];
      constructMap[procI].setSize(nRecv);
      for (label i = 0; i < nRecv; i++) {
        constructMap[procI][i] = segmentI++;
      }
    }
  }
  // Pack into distribution map
  autoPtr<mapDistribute> mapPtr
  {
    new mapDistribute
    {
      segmentI,       // size after construction
      sendMap.xfer(),
      constructMap.xfer()
    }
  };
  const mapDistribute& map = mapPtr();
  // Send over queries
  // ~~~~~~~~~~~~~~~~~
  map.distribute(triangleIndex);
  return mapPtr;
}


mousse::label mousse::distributedTriSurfaceMesh::calcOverlappingProcs
(
  const point& centre,
  const scalar radiusSqr,
  boolList& overlaps
) const
{
  overlaps = false;
  label nOverlaps = 0;
  FOR_ALL(procBb_, procI) {
    const List<treeBoundBox>& bbs = procBb_[procI];
    FOR_ALL(bbs, bbI) {
      if (bbs[bbI].overlaps(centre, radiusSqr)) {
        overlaps[procI] = true;
        nOverlaps++;
        break;
      }
    }
  }
  return nOverlaps;
}


// Generate queries for parallel distance calculation
// - calculates exchange map
// - uses map to exchange points and radius
mousse::autoPtr<mousse::mapDistribute>
mousse::distributedTriSurfaceMesh::calcLocalQueries
(
  const pointField& centres,
  const scalarField& radiusSqr,
  pointField& allCentres,
  scalarField& allRadiusSqr,
  labelList& allSegmentMap
) const
{
  // Determine queries
  // ~~~~~~~~~~~~~~~~~
  labelListList sendMap{Pstream::nProcs()};

  {
    // Queries
    DynamicList<point> dynAllCentres{centres.size()};
    DynamicList<scalar> dynAllRadiusSqr{centres.size()};
    // Original index of segment
    DynamicList<label> dynAllSegmentMap{centres.size()};
    // Per processor indices into allSegments to send
    List<DynamicList<label>> dynSendMap{Pstream::nProcs()};
    // Work array - whether processor bb overlaps the bounding sphere.
    boolList procBbOverlaps{Pstream::nProcs()};
    FOR_ALL(centres, centreI) {
      // Find the processor this sample+radius overlaps.
      calcOverlappingProcs
        (
          centres[centreI],
          radiusSqr[centreI],
          procBbOverlaps
        );
      FOR_ALL(procBbOverlaps, procI) {
        if (procI != Pstream::myProcNo() && procBbOverlaps[procI]) {
          dynSendMap[procI].append(dynAllCentres.size());
          dynAllSegmentMap.append(centreI);
          dynAllCentres.append(centres[centreI]);
          dynAllRadiusSqr.append(radiusSqr[centreI]);
        }
      }
    }
    // Convert dynamicList to labelList
    sendMap.setSize(Pstream::nProcs());
    FOR_ALL(sendMap, procI) {
      dynSendMap[procI].shrink();
      sendMap[procI].transfer(dynSendMap[procI]);
    }
    allCentres.transfer(dynAllCentres.shrink());
    allRadiusSqr.transfer(dynAllRadiusSqr.shrink());
    allSegmentMap.transfer(dynAllSegmentMap.shrink());
  }
  // Send over how many I need to receive.
  labelListList sendSizes{Pstream::nProcs()};
  sendSizes[Pstream::myProcNo()].setSize(Pstream::nProcs());
  FOR_ALL(sendMap, procI) {
    sendSizes[Pstream::myProcNo()][procI] = sendMap[procI].size();
  }
  Pstream::gatherList(sendSizes);
  Pstream::scatterList(sendSizes);
  // Determine order of receiving
  labelListList constructMap{Pstream::nProcs()};
  // My local segments first
  constructMap[Pstream::myProcNo()] =
    identity(sendMap[Pstream::myProcNo()].size());
  label segmentI = constructMap[Pstream::myProcNo()].size();
  FOR_ALL(constructMap, procI) {
    if (procI != Pstream::myProcNo()) {
      // What I need to receive is what other processor is sending to me.
      label nRecv = sendSizes[procI][Pstream::myProcNo()];
      constructMap[procI].setSize(nRecv);
      for (label i = 0; i < nRecv; i++) {
        constructMap[procI][i] = segmentI++;
      }
    }
  }
  autoPtr<mapDistribute> mapPtr
  {
    new mapDistribute
    {
      segmentI,       // size after construction
      sendMap.xfer(),
      constructMap.xfer()
    }
  };
  return mapPtr;
}


// Find bounding boxes that guarantee a more or less uniform distribution
// of triangles. Decomposition in here is only used to get the bounding
// boxes, actual decomposition is done later on.
// Returns a per processor a list of bounding boxes that most accurately
// describe the shape. For now just a single bounding box per processor but
// optimisation might be to determine a better fitting shape.
mousse::List<mousse::List<mousse::treeBoundBox>>
mousse::distributedTriSurfaceMesh::independentlyDistributedBbs
(
  const triSurface& s
)
{
  if (!decomposer_.valid()) {
    // Use current decomposer.
    // Note: or always use hierarchical?
    IOdictionary decomposeDict
    {
      {
        "decomposeParDict",
        searchableSurface::time().system(),
        searchableSurface::time(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      }
    };
    decomposer_ = decompositionMethod::New(decomposeDict);
    if (!decomposer_().parallelAware()) {
      FATAL_ERROR_IN
      (
        "distributedTriSurfaceMesh::independentlyDistributedBbs"
        "(const triSurface&)"
      )
      << "The decomposition method " << decomposer_().typeName
      << " does not decompose in parallel."
      << " Please choose one that does." << exit(FatalError);
    }
    if (!isA<geomDecomp>(decomposer_())) {
      FATAL_ERROR_IN
      (
        "distributedTriSurfaceMesh::independentlyDistributedBbs"
        "(const triSurface&)"
      )
      << "The decomposition method " << decomposer_().typeName
      << " is not a geometric decomposition method." << endl
      << "Only geometric decomposition methods are currently"
      << " supported."
      << exit(FatalError);
    }
  }
  // Do decomposition according to triangle centre
  pointField triCentres{s.size()};
  FOR_ALL(s, triI) {
    triCentres[triI] = s[triI].centre(s.points());
  }
  geomDecomp& decomposer = refCast<geomDecomp>(decomposer_());
  // Do the actual decomposition
  labelList distribution{decomposer.decompose(triCentres)};
  // Find bounding box for all triangles on new distribution.
  // Initialise to inverted box (VGREAT, -VGREAT)
  List<List<treeBoundBox>> bbs{Pstream::nProcs()};
  FOR_ALL(bbs, procI) {
    bbs[procI].setSize(1);
    //bbs[procI][0] = boundBox::invertedBox;
    bbs[procI][0].min() = point( VGREAT,  VGREAT,  VGREAT);
    bbs[procI][0].max() = point(-VGREAT, -VGREAT, -VGREAT);
  }
  FOR_ALL(s, triI) {
    point& bbMin = bbs[distribution[triI]][0].min();
    point& bbMax = bbs[distribution[triI]][0].max();
    const triSurface::FaceType& f = s[triI];
    FOR_ALL(f, fp) {
      const point& pt = s.points()[f[fp]];
      bbMin = ::mousse::min(bbMin, pt);
      bbMax = ::mousse::max(bbMax, pt);
    }
  }
  // Now combine for all processors and convert to correct format.
  FOR_ALL(bbs, procI) {
    FOR_ALL(bbs[procI], i) {
      reduce(bbs[procI][i].min(), minOp<point>());
      reduce(bbs[procI][i].max(), maxOp<point>());
    }
  }
  return bbs;
}


// Does any part of triangle overlap bb.
bool mousse::distributedTriSurfaceMesh::overlaps
(
  const List<treeBoundBox>& bbs,
  const point& p0,
  const point& p1,
  const point& p2
)
{
  FOR_ALL(bbs, bbI) {
    const treeBoundBox& bb = bbs[bbI];
    treeBoundBox triBb{p0, p0};
    triBb.min() = min(triBb.min(), p1);
    triBb.min() = min(triBb.min(), p2);
    triBb.max() = max(triBb.max(), p1);
    triBb.max() = max(triBb.max(), p2);
    //- Exact test of triangle intersecting bb
    // Quick rejection. If whole bounding box of tri is outside cubeBb then
    // there will be no intersection.
    if (!bb.overlaps(triBb))
      continue;
    // Check if one or more triangle point inside
    if (bb.contains(p0) || bb.contains(p1) || bb.contains(p2)) {
      // One or more points inside
      return true;
    }
    // Now we have the difficult case: all points are outside but
    // connecting edges might go through cube. Use fast intersection
    // of bounding box.
    bool intersect = triangleFuncs::intersectBb(p0, p1, p2, bb);
    if (intersect) {
      return true;
    }
  }
  return false;
}


void mousse::distributedTriSurfaceMesh::subsetMeshMap
(
  const triSurface& s,
  const boolList& include,
  const label nIncluded,
  labelList& newToOldPoints,
  labelList& oldToNewPoints,
  labelList& newToOldFaces
)
{
  newToOldFaces.setSize(nIncluded);
  newToOldPoints.setSize(s.points().size());
  oldToNewPoints.setSize(s.points().size());
  oldToNewPoints = -1;

  {
    label faceI = 0;
    label pointI = 0;
    FOR_ALL(include, oldFacei) {
      if (include[oldFacei]) {
        // Store new faces compact
        newToOldFaces[faceI++] = oldFacei;
        // Renumber labels for face
        const triSurface::FaceType& f = s[oldFacei];
        FOR_ALL(f, fp) {
          label oldPointI = f[fp];
          if (oldToNewPoints[oldPointI] == -1) {
            oldToNewPoints[oldPointI] = pointI;
            newToOldPoints[pointI++] = oldPointI;
          }
        }
      }
    }
    newToOldPoints.setSize(pointI);
  }
}


mousse::triSurface mousse::distributedTriSurfaceMesh::subsetMesh
(
  const triSurface& s,
  const labelList& newToOldPoints,
  const labelList& oldToNewPoints,
  const labelList& newToOldFaces
)
{
  // Extract points
  pointField newPoints{newToOldPoints.size()};
  FOR_ALL(newToOldPoints, i) {
    newPoints[i] = s.points()[newToOldPoints[i]];
  }
  // Extract faces
  List<labelledTri> newTriangles{newToOldFaces.size()};
  FOR_ALL(newToOldFaces, i) {
    // Get old vertex labels
    const labelledTri& tri = s[newToOldFaces[i]];
    newTriangles[i][0] = oldToNewPoints[tri[0]];
    newTriangles[i][1] = oldToNewPoints[tri[1]];
    newTriangles[i][2] = oldToNewPoints[tri[2]];
    newTriangles[i].region() = tri.region();
  }
  // Reuse storage.
  return {newTriangles, s.patches(), newPoints, true};
}


mousse::triSurface mousse::distributedTriSurfaceMesh::subsetMesh
(
  const triSurface& s,
  const boolList& include,
  labelList& newToOldPoints,
  labelList& newToOldFaces
)
{
  label n = 0;
  FOR_ALL(include, i) {
    if (include[i]) {
      n++;
    }
  }
  labelList oldToNewPoints;
  subsetMeshMap
    (
      s,
      include,
      n,
      newToOldPoints,
      oldToNewPoints,
      newToOldFaces
    );
  return subsetMesh(s, newToOldPoints, oldToNewPoints, newToOldFaces);
}


mousse::triSurface mousse::distributedTriSurfaceMesh::subsetMesh
(
  const triSurface& s,
  const labelList& newToOldFaces,
  labelList& newToOldPoints
)
{
  const boolList include
  {
    createWithValues<boolList>
    (
      s.size(),
      false,
      newToOldFaces,
      true
    )
  };
  newToOldPoints.setSize(s.points().size());
  labelList oldToNewPoints{s.points().size(), -1};

  {
    label pointI = 0;
    FOR_ALL(include, oldFacei) {
      if (include[oldFacei]) {
        // Renumber labels for face
        const triSurface::FaceType& f = s[oldFacei];
        FOR_ALL(f, fp) {
          label oldPointI = f[fp];
          if (oldToNewPoints[oldPointI] == -1) {
            oldToNewPoints[oldPointI] = pointI;
            newToOldPoints[pointI++] = oldPointI;
          }
        }
      }
    }
    newToOldPoints.setSize(pointI);
  }
  return subsetMesh(s, newToOldPoints, oldToNewPoints, newToOldFaces);
}


mousse::label mousse::distributedTriSurfaceMesh::findTriangle
(
  const List<labelledTri>& allFaces,
  const labelListList& allPointFaces,
  const labelledTri& otherF
)
{
  // allFaces connected to otherF[0]
  const labelList& pFaces = allPointFaces[otherF[0]];
  FOR_ALL(pFaces, i) {
    const labelledTri& f = allFaces[pFaces[i]];
    if (f.region() == otherF.region()) {
      // Find index of otherF[0]
      label fp0 = findIndex(f, otherF[0]);
      // Check rest of triangle in same order
      label fp1 = f.fcIndex(fp0);
      label fp2 = f.fcIndex(fp1);
      if (f[fp1] == otherF[1] && f[fp2] == otherF[2]) {
        return pFaces[i];
      }
    }
  }
  return -1;
}


// Merge into allSurf.
void mousse::distributedTriSurfaceMesh::merge
(
  const scalar mergeDist,
  const List<labelledTri>& subTris,
  const pointField& subPoints,
  List<labelledTri>& allTris,
  pointField& allPoints,
  labelList& faceConstructMap,
  labelList& pointConstructMap
)
{
  labelList subToAll;
  matchPoints
    (
      subPoints,
      allPoints,
      scalarField(subPoints.size(), mergeDist),   // match distance
      false,                                      // verbose
      pointConstructMap
    );
  label nOldAllPoints = allPoints.size();
  // Add all unmatched points
  label allPointI = nOldAllPoints;
  FOR_ALL(pointConstructMap, pointI) {
    if (pointConstructMap[pointI] == -1) {
      pointConstructMap[pointI] = allPointI++;
    }
  }
  if (allPointI > nOldAllPoints) {
    allPoints.setSize(allPointI);
    FOR_ALL(pointConstructMap, pointI) {
      if (pointConstructMap[pointI] >= nOldAllPoints) {
        allPoints[pointConstructMap[pointI]] = subPoints[pointI];
      }
    }
  }
  // To check whether triangles are same we use pointFaces.
  labelListList allPointFaces;
  invertManyToMany(nOldAllPoints, allTris, allPointFaces);
  // Add all unmatched triangles
  label allTriI = allTris.size();
  allTris.setSize(allTriI + subTris.size());
  faceConstructMap.setSize(subTris.size());
  FOR_ALL(subTris, triI) {
    const labelledTri& subTri = subTris[triI];
    // Get triangle in new numbering
    labelledTri mappedTri
    {
      pointConstructMap[subTri[0]],
      pointConstructMap[subTri[1]],
      pointConstructMap[subTri[2]],
      subTri.region()
    };
    // Check if all points were already in surface
    bool fullMatch = true;
    FOR_ALL(mappedTri, fp) {
      if (mappedTri[fp] >= nOldAllPoints) {
        fullMatch = false;
        break;
      }
    }
    if (fullMatch) {
      // All three points are mapped to old points. See if same
      // triangle.
      label i = findTriangle(allTris, allPointFaces, mappedTri);
      if (i == -1) {
        // Add
        faceConstructMap[triI] = allTriI;
        allTris[allTriI] = mappedTri;
        allTriI++;
      } else {
        faceConstructMap[triI] = i;
      }
    } else {
      // Add
      faceConstructMap[triI] = allTriI;
      allTris[allTriI] = mappedTri;
      allTriI++;
    }
  }
  allTris.setSize(allTriI);
}


// Constructors 
mousse::distributedTriSurfaceMesh::distributedTriSurfaceMesh
(
  const IOobject& io,
  const triSurface& s,
  const dictionary& dict
)
:
  triSurfaceMesh{io, s},
  dict_
  {
    {
      searchableSurface::name() + "Dict",
      searchableSurface::instance(),
      searchableSurface::local(),
      searchableSurface::db(),
      searchableSurface::NO_READ,
      searchableSurface::writeOpt(),
      searchableSurface::registerObject()
    },
    dict
  }
{
  read();
  reduce(bounds().min(), minOp<point>());
  reduce(bounds().max(), maxOp<point>());
  if (debug) {
    Info << "Constructed from triSurface:" << endl;
    writeStats(Info);
    labelList nTris{Pstream::nProcs()};
    nTris[Pstream::myProcNo()] = triSurface::size();
    Pstream::gatherList(nTris);
    Pstream::scatterList(nTris);
    Info << endl<< "\tproc\ttris\tbb" << endl;
    FOR_ALL(nTris, procI) {
      Info << '\t' << procI << '\t' << nTris[procI]
        << '\t' << procBb_[procI] << endl;
    }
    Info << endl;
  }
}


mousse::distributedTriSurfaceMesh::distributedTriSurfaceMesh(const IOobject& io)
:
  triSurfaceMesh
  {
    {
      io.name(),
      io.time().findInstance(io.local(), word::null),
      io.local(),
      io.db(),
      io.readOpt(),
      io.writeOpt(),
      io.registerObject()
    }
  },
  dict_
  {
    {
      searchableSurface::name() + "Dict",
      searchableSurface::instance(),
      searchableSurface::local(),
      searchableSurface::db(),
      searchableSurface::readOpt(),
      searchableSurface::writeOpt(),
      searchableSurface::registerObject()
    }
  }
{
  if (Pstream::parRun()
      && (dict_.readOpt() == IOobject::MUST_READ
          || dict_.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      && (regIOobject::fileModificationChecking == timeStampMaster
          || regIOobject::fileModificationChecking == inotifyMaster)) {
    FATAL_ERROR_IN("mousse::distributedTriSurfaceMesh::read()")
      << "    distributedTriSurfaceMesh is being constructed\n"
      << "    using 'timeStampMaster' or 'inotifyMaster.'\n"
      << "    Modify the entry fileModificationChecking\n"
      << "    in the etc/controlDict.\n"
      << "    Use 'timeStamp' instead."
      << exit(FatalError);
  }
  read();
  reduce(bounds().min(), minOp<point>());
  reduce(bounds().max(), maxOp<point>());
  if (debug) {
    Info << "Read distributedTriSurface from " << io.objectPath()
      << ':' << endl;
    writeStats(Info);
    labelList nTris{Pstream::nProcs()};
    nTris[Pstream::myProcNo()] = triSurface::size();
    Pstream::gatherList(nTris);
    Pstream::scatterList(nTris);
    Info << endl<< "\tproc\ttris\tbb" << endl;
    FOR_ALL(nTris, procI) {
      Info << '\t' << procI << '\t' << nTris[procI]
        << '\t' << procBb_[procI] << endl;
    }
    Info << endl;
  }
}


mousse::distributedTriSurfaceMesh::distributedTriSurfaceMesh
(
  const IOobject& io,
  const dictionary& dict
)
:
  //triSurfaceMesh(io, dict),
  triSurfaceMesh
  {
    {
      io.name(),
      io.time().findInstance(io.local(), word::null),
      io.local(),
      io.db(),
      io.readOpt(),
      io.writeOpt(),
      io.registerObject()
    },
    dict
  },
  dict_
  {
    {
      searchableSurface::name() + "Dict",
      searchableSurface::instance(),
      searchableSurface::local(),
      searchableSurface::db(),
      searchableSurface::readOpt(),
      searchableSurface::writeOpt(),
      searchableSurface::registerObject()
    }
  }
{
  if (Pstream::parRun()
      && (dict_.readOpt() == IOobject::MUST_READ
          || dict_.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      && (regIOobject::fileModificationChecking == timeStampMaster
          || regIOobject::fileModificationChecking == inotifyMaster)) {
    FATAL_ERROR_IN("mousse::distributedTriSurfaceMesh::read()")
      << "    distributedTriSurfaceMesh is being constructed\n"
      << "    using 'timeStampMaster' or 'inotifyMaster.'\n"
      << "    Modify the entry fileModificationChecking\n"
      << "    in the etc/controlDict.\n"
      << "    Use 'timeStamp' instead."
      << exit(FatalError);
  }
  read();
  reduce(bounds().min(), minOp<point>());
  reduce(bounds().max(), maxOp<point>());
  if (debug) {
    Info << "Read distributedTriSurface from " << io.objectPath()
      << " and dictionary:" << endl;
    writeStats(Info);
    labelList nTris{Pstream::nProcs()};
    nTris[Pstream::myProcNo()] = triSurface::size();
    Pstream::gatherList(nTris);
    Pstream::scatterList(nTris);
    Info << endl<< "\tproc\ttris\tbb" << endl;
    FOR_ALL(nTris, procI) {
      Info << '\t' << procI << '\t' << nTris[procI]
        << '\t' << procBb_[procI] << endl;
    }
    Info << endl;
  }
}


// Destructor 
mousse::distributedTriSurfaceMesh::~distributedTriSurfaceMesh()
{
  clearOut();
}


void mousse::distributedTriSurfaceMesh::clearOut()
{
  globalTris_.clear();
  triSurfaceMesh::clearOut();
}


// Member Functions 
const mousse::globalIndex& mousse::distributedTriSurfaceMesh::globalTris() const
{
  if (!globalTris_.valid()) {
    globalTris_.reset(new globalIndex(triSurface::size()));
  }
  return globalTris_;
}


void mousse::distributedTriSurfaceMesh::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  List<pointIndexHit>& info
) const
{
  const indexedOctree<treeDataTriSurface>& octree = tree();
  // Important:force synchronised construction of indexing
  const globalIndex& triIndexer = globalTris();
  // Initialise
  // ~~~~~~~~~~
  info.setSize(samples.size());
  FOR_ALL(info, i) {
    info[i].setMiss();
  }
  // Do any local queries
  // ~~~~~~~~~~~~~~~~~~~~
  label nLocal = 0;

  {
    // Work array - whether processor bb overlaps the bounding sphere.
    boolList procBbOverlaps{Pstream::nProcs()};
    FOR_ALL(samples, i) {
      // Find the processor this sample+radius overlaps.
      label nProcs =
        calcOverlappingProcs(samples[i], nearestDistSqr[i], procBbOverlaps);
      // Overlaps local processor?
      if (procBbOverlaps[Pstream::myProcNo()]) {
        info[i] = octree.findNearest(samples[i], nearestDistSqr[i]);
        if (info[i].hit()) {
          info[i].setIndex(triIndexer.toGlobal(info[i].index()));
        }
        if (nProcs == 1) {
          // Fully local
          nLocal++;
        }
      }
    }
  }
  const label l1 = returnReduce(nLocal, sumOp<label>());
  const label l2 = returnReduce(samples.size(), sumOp<label>());
  if (Pstream::parRun() && (l1 < l2)) {
    // Not all can be resolved locally. Build queries and map, send over
    // queries, do intersections, send back and merge.
    // Calculate queries and exchange map
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    pointField allCentres;
    scalarField allRadiusSqr;
    labelList allSegmentMap;
    autoPtr<mapDistribute> mapPtr
    {
      calcLocalQueries
        (
          samples,
          nearestDistSqr,
          allCentres,
          allRadiusSqr,
          allSegmentMap
        )
    };
    const mapDistribute& map = mapPtr();
    // swap samples to local processor
    map.distribute(allCentres);
    map.distribute(allRadiusSqr);
    // Do my tests
    List<pointIndexHit> allInfo{allCentres.size()};
    FOR_ALL(allInfo, i) {
      allInfo[i] = octree.findNearest(allCentres[i], allRadiusSqr[i]);
      if (allInfo[i].hit()) {
        allInfo[i].setIndex(triIndexer.toGlobal(allInfo[i].index()));
      }
    }
    // Send back results
    map.reverseDistribute(allSegmentMap.size(), allInfo);
    // Extract information
    FOR_ALL(allInfo, i) {
      if (allInfo[i].hit()) {
        label pointI = allSegmentMap[i];
        if (!info[pointI].hit()) {
          // No intersection yet so take this one
          info[pointI] = allInfo[i];
        } else {
          // Nearest intersection
          const scalar r1 = magSqr(allInfo[i].hitPoint() - samples[pointI]);
          const scalar r2 = magSqr(info[pointI].hitPoint() - samples[pointI]);
          if (r1 < r2) {
            info[pointI] = allInfo[i];
          }
        }
      }
    }
  }
}


void mousse::distributedTriSurfaceMesh::findLine
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  findLine
    (
      true,   // nearestIntersection
      start,
      end,
      info
    );
}


void mousse::distributedTriSurfaceMesh::findLineAny
(
  const pointField& start,
  const pointField& end,
  List<pointIndexHit>& info
) const
{
  findLine
    (
      true,   // nearestIntersection
      start,
      end,
      info
    );
}


void mousse::distributedTriSurfaceMesh::findLineAll
(
  const pointField& start,
  const pointField& end,
  List<List<pointIndexHit>>& info
) const
{
  // Reuse fineLine. We could modify all of findLine to do multiple
  // intersections but this would mean a lot of data transferred so
  // for now we just find nearest intersection and retest from that
  // intersection onwards.
  // Work array.
  List<pointIndexHit> hitInfo{start.size()};
  findLine
    (
      true,   // nearestIntersection
      start,
      end,
      hitInfo
    );
  // Tolerances:
  // To find all intersections we add a small vector to the last intersection
  // This is chosen such that
  // - it is significant (SMALL is smallest representative relative tolerance;
  //   we need something bigger since we're doing calculations)
  // - if the start-end vector is zero we still progress
  const vectorField dirVec{end-start};
  const scalarField magSqrDirVec{magSqr(dirVec)};
  const vectorField
    smallVec{ROOTSMALL*dirVec + vector(ROOTVSMALL,ROOTVSMALL,ROOTVSMALL)};
  // Copy to input and compact any hits
  labelList pointMap{start.size()};
  pointField e0{start.size()};
  pointField e1{start.size()};
  label compactI = 0;
  info.setSize(hitInfo.size());
  FOR_ALL(hitInfo, pointI) {
    if (hitInfo[pointI].hit()) {
      info[pointI].setSize(1);
      info[pointI][0] = hitInfo[pointI];
      point pt = hitInfo[pointI].hitPoint() + smallVec[pointI];
      if (((pt - start[pointI]) & dirVec[pointI]) <= magSqrDirVec[pointI]) {
        e0[compactI] = pt;
        e1[compactI] = end[pointI];
        pointMap[compactI] = pointI;
        compactI++;
      }
    } else {
      info[pointI].clear();
    }
  }
  e0.setSize(compactI);
  e1.setSize(compactI);
  pointMap.setSize(compactI);
  while (returnReduce(e0.size(), sumOp<label>()) > 0) {
    findLine
      (
        true,   // nearestIntersection
        e0,
        e1,
        hitInfo
      );
    // Extract
    label compactI = 0;
    FOR_ALL(hitInfo, i) {
      if (hitInfo[i].hit()) {
        label pointI = pointMap[i];
        label sz = info[pointI].size();
        info[pointI].setSize(sz+1);
        info[pointI][sz] = hitInfo[i];
        point pt = hitInfo[i].hitPoint() + smallVec[pointI];
        if (((pt-start[pointI])&dirVec[pointI]) <= magSqrDirVec[pointI]) {
          e0[compactI] = pt;
          e1[compactI] = end[pointI];
          pointMap[compactI] = pointI;
          compactI++;
        }
      }
    }
    // Trim
    e0.setSize(compactI);
    e1.setSize(compactI);
    pointMap.setSize(compactI);
  }
}


void mousse::distributedTriSurfaceMesh::getRegion
(
  const List<pointIndexHit>& info,
  labelList& region
) const
{
  if (!Pstream::parRun()) {
    region.setSize(info.size());
    FOR_ALL(info, i) {
      if (info[i].hit()) {
        region[i] = triSurface::operator[](info[i].index()).region();
      } else {
        region[i] = -1;
      }
    }
    return;
  }
  // Get query data (= local index of triangle)
  labelList triangleIndex{info.size()};
  autoPtr<mapDistribute> mapPtr{calcLocalQueries(info, triangleIndex)};
  const mapDistribute& map = mapPtr();
  // Do my tests
  const triSurface& s = static_cast<const triSurface&>(*this);
  region.setSize(triangleIndex.size());
  FOR_ALL(triangleIndex, i) {
    label triI = triangleIndex[i];
    region[i] = s[triI].region();
  }
  // Send back results
  map.reverseDistribute(info.size(), region);
}


void mousse::distributedTriSurfaceMesh::getNormal
(
  const List<pointIndexHit>& info,
  vectorField& normal
) const
{
  if (!Pstream::parRun()) {
    triSurfaceMesh::getNormal(info, normal);
    return;
  }
  // Get query data (= local index of triangle)
  labelList triangleIndex{info.size()};
  autoPtr<mapDistribute> mapPtr{calcLocalQueries(info, triangleIndex)};
  const mapDistribute& map = mapPtr();
  // Do my tests
  const triSurface& s = static_cast<const triSurface&>(*this);
  normal.setSize(triangleIndex.size());
  FOR_ALL(triangleIndex, i) {
    label triI = triangleIndex[i];
    normal[i] = s[triI].normal(s.points());
    normal[i] /= mag(normal[i]) + VSMALL;
  }
  // Send back results
  map.reverseDistribute(info.size(), normal);
}


void mousse::distributedTriSurfaceMesh::getField
(
  const List<pointIndexHit>& info,
  labelList& values
) const
{
  if (!Pstream::parRun()) {
    triSurfaceMesh::getField(info, values);
    return;
  }
  if (foundObject<triSurfaceLabelField>("values")) {
    const triSurfaceLabelField& fld =
      lookupObject<triSurfaceLabelField>("values");
    // Get query data (= local index of triangle)
    labelList triangleIndex{info.size()};
    autoPtr<mapDistribute> mapPtr{calcLocalQueries(info, triangleIndex)};
    const mapDistribute& map = mapPtr();
    // Do my tests
    values.setSize(triangleIndex.size());
    FOR_ALL(triangleIndex, i) {
      label triI = triangleIndex[i];
      values[i] = fld[triI];
    }
    // Send back results
    map.reverseDistribute(info.size(), values);
  }
}


void mousse::distributedTriSurfaceMesh::getVolumeType
(
  const pointField& /*points*/,
  List<volumeType>& /*volType*/
) const
{
  FATAL_ERROR_IN
  (
    "distributedTriSurfaceMesh::getVolumeType"
    "(const pointField&, List<volumeType>&) const"
  )
  << "Volume type not supported for distributed surfaces."
  << exit(FatalError);
}


// Subset the part of surface that is overlapping bb.
mousse::triSurface mousse::distributedTriSurfaceMesh::overlappingSurface
(
  const triSurface& s,
  const List<treeBoundBox>& bbs,
  labelList& subPointMap,
  labelList& subFaceMap
)
{
  // Determine what triangles to keep.
  boolList includedFace{s.size(), false};
  // Create a slightly larger bounding box.
  List<treeBoundBox> bbsX{bbs.size()};
  const scalar eps = 1.0e-4;
  FOR_ALL(bbs, i) {
    const point mid = 0.5*(bbs[i].min() + bbs[i].max());
    const vector halfSpan = (1.0+eps)*(bbs[i].max() - mid);
    bbsX[i].min() = mid - halfSpan;
    bbsX[i].max() = mid + halfSpan;
  }
  FOR_ALL(s, triI) {
    const labelledTri& f = s[triI];
    const point& p0 = s.points()[f[0]];
    const point& p1 = s.points()[f[1]];
    const point& p2 = s.points()[f[2]];
    if (overlaps(bbsX, p0, p1, p2)) {
      includedFace[triI] = true;
    }
  }
  return subsetMesh(s, includedFace, subPointMap, subFaceMap);
}


void mousse::distributedTriSurfaceMesh::distribute
(
  const List<treeBoundBox>& bbs,
  const bool keepNonLocal,
  autoPtr<mapDistribute>& faceMap,
  autoPtr<mapDistribute>& pointMap
)
{
  // Get bbs of all domains
  {
    List<List<treeBoundBox>> newProcBb{Pstream::nProcs()};
    switch(distType_) {
      case FOLLOW:
        newProcBb[Pstream::myProcNo()].setSize(bbs.size());
        FOR_ALL(bbs, i) {
          newProcBb[Pstream::myProcNo()][i] = bbs[i];
        }
        Pstream::gatherList(newProcBb);
        Pstream::scatterList(newProcBb);
      break;
      case INDEPENDENT:
        newProcBb = independentlyDistributedBbs(*this);
      break;
      case FROZEN:
        return;
      break;
      default:
        FATAL_ERROR_IN("distributedTriSurfaceMesh::distribute(..)")
          << "Unsupported distribution type." << exit(FatalError);
      break;
    }
    if (newProcBb == procBb_) {
      return;
    } else {
      procBb_.transfer(newProcBb);
      dict_.set("bounds", procBb_[Pstream::myProcNo()]);
    }
  }
  // Debug information
  if (debug) {
    labelList nTris{Pstream::nProcs()};
    nTris[Pstream::myProcNo()] = triSurface::size();
    Pstream::gatherList(nTris);
    Pstream::scatterList(nTris);
    Info << "distributedTriSurfaceMesh::distribute : before distribution:"
      << endl
      << "\tproc\ttris" << endl;
    FOR_ALL(nTris, procI) {
      Info << '\t' << procI << '\t' << nTris[procI] << endl;
    }
    Info << endl;
  }
  // Use procBbs to determine which faces go where
  labelListList faceSendMap{Pstream::nProcs()};
  labelListList pointSendMap{Pstream::nProcs()};
  FOR_ALL(procBb_, procI) {
    overlappingSurface
      (
        *this,
        procBb_[procI],
        pointSendMap[procI],
        faceSendMap[procI]
      );
  }
  if (keepNonLocal) {
    // Include in faceSendMap/pointSendMap the triangles that are
    // not mapped to any processor so they stay local.
    const triSurface& s = static_cast<const triSurface&>(*this);
    boolList includedFace{s.size(), true};
    FOR_ALL(faceSendMap, procI) {
      if (procI != Pstream::myProcNo()) {
        FOR_ALL(faceSendMap[procI], i) {
          includedFace[faceSendMap[procI][i]] = false;
        }
      }
    }
    // Combine includedFace (all triangles that are not on any neighbour)
    // with overlap.
    FOR_ALL(faceSendMap[Pstream::myProcNo()], i) {
      includedFace[faceSendMap[Pstream::myProcNo()][i]] = true;
    }
    subsetMesh
      (
        s,
        includedFace,
        pointSendMap[Pstream::myProcNo()],
        faceSendMap[Pstream::myProcNo()]
      );
  }
  // Send over how many faces/points I need to receive
  labelListList faceSendSizes{Pstream::nProcs()};
  faceSendSizes[Pstream::myProcNo()].setSize(Pstream::nProcs());
  FOR_ALL(faceSendMap, procI) {
    faceSendSizes[Pstream::myProcNo()][procI] = faceSendMap[procI].size();
  }
  Pstream::gatherList(faceSendSizes);
  Pstream::scatterList(faceSendSizes);
  // Exchange surfaces
  // Storage for resulting surface
  List<labelledTri> allTris;
  pointField allPoints;
  labelListList faceConstructMap{Pstream::nProcs()};
  labelListList pointConstructMap{Pstream::nProcs()};
  // My own surface first
  {
    labelList pointMap;
    triSurface
      subSurface{subsetMesh(*this, faceSendMap[Pstream::myProcNo()], pointMap)};
    allTris = subSurface;
    allPoints = subSurface.points();
    faceConstructMap[Pstream::myProcNo()] =
      identity(faceSendMap[Pstream::myProcNo()].size());
    pointConstructMap[Pstream::myProcNo()] =
      identity(pointSendMap[Pstream::myProcNo()].size());
  }
  // Send all
  FOR_ALL(faceSendSizes, procI) {
    if (procI != Pstream::myProcNo()) {
      if (faceSendSizes[Pstream::myProcNo()][procI] > 0) {
        OPstream str{Pstream::blocking, static_cast<int>(procI)};
        labelList pointMap;
        triSurface
          subSurface{subsetMesh(*this, faceSendMap[procI], pointMap)};
        str << subSurface;
     }
    }
  }
  // Receive and merge all
  FOR_ALL(faceSendSizes, procI) {
    if (procI != Pstream::myProcNo()) {
      if (faceSendSizes[procI][Pstream::myProcNo()] > 0) {
        IPstream str{Pstream::blocking, static_cast<int>(procI)};
        // Receive
        triSurface subSurface{str};
        merge
          (
            mergeDist_,
            subSurface,
            subSurface.points(),
            allTris,
            allPoints,
            faceConstructMap[procI],
            pointConstructMap[procI]
          );
      }
    }
  }
  faceMap.reset
    (
      new mapDistribute
      {
        allTris.size(),
        faceSendMap.xfer(),
        faceConstructMap.xfer()
      }
    );
  pointMap.reset
    (
      new mapDistribute
      {
        allPoints.size(),
        pointSendMap.xfer(),
        pointConstructMap.xfer()
      }
    );
  // Construct triSurface. Reuse storage.
  triSurface::operator=(triSurface{allTris, patches(), allPoints, true});
  clearOut();
  // Set the bounds() value to the boundBox of the undecomposed surface
  triSurfaceMesh::bounds() = boundBox{points()};
  reduce(bounds().min(), minOp<point>());
  reduce(bounds().max(), maxOp<point>());
  // Regions stays same
  // Volume type stays same.
  distributeFields<label>(faceMap());
  distributeFields<scalar>(faceMap());
  distributeFields<vector>(faceMap());
  distributeFields<sphericalTensor>(faceMap());
  distributeFields<symmTensor>(faceMap());
  distributeFields<tensor>(faceMap());
  if (debug) {
    labelList nTris(Pstream::nProcs());
    nTris[Pstream::myProcNo()] = triSurface::size();
    Pstream::gatherList(nTris);
    Pstream::scatterList(nTris);
    Info << "distributedTriSurfaceMesh::distribute : after distribution:"
      << endl
      << "\tproc\ttris" << endl;
    FOR_ALL(nTris, procI) {
      Info << '\t' << procI << '\t' << nTris[procI] << endl;
    }
    Info << endl;
  }
}


//- Write using given format, version and compression
bool mousse::distributedTriSurfaceMesh::writeObject
(
  IOstream::streamFormat /*fmt*/,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  // Make sure dictionary goes to same directory as surface
  const_cast<fileName&>(dict_.instance()) = searchableSurface::instance();
  // Copy of triSurfaceMesh::writeObject except for the sorting of
  // triangles by region. This is done so we preserve region names,
  // even if locally we have zero triangles.
  {
    fileName fullPath{searchableSurface::objectPath()};
    if (!mkDir(fullPath.path())) {
      return false;
    }
    // Important: preserve any zero-sized patches
    triSurface::write(fullPath, true);
    if (!isFile(fullPath)) {
      return false;
    }
  }
  // Dictionary needs to be written in ascii - binary output not supported.
  return dict_.writeObject(IOstream::ASCII, ver, cmp);
}


void mousse::distributedTriSurfaceMesh::writeStats(Ostream& os) const
{
  boundBox bb;
  label nPoints;
  PatchTools::calcBounds(static_cast<const triSurface&>(*this), bb, nPoints);
  reduce(bb.min(), minOp<point>());
  reduce(bb.max(), maxOp<point>());
  os << "Triangles    : " << returnReduce(triSurface::size(), sumOp<label>())
    << endl
    << "Vertices     : " << returnReduce(nPoints, sumOp<label>()) << endl
    << "Bounding Box : " << bb << endl;
}

