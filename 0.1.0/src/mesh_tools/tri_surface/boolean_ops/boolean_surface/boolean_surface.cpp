// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "boolean_surface.hpp"
#include "intersected_surface.hpp"
#include "oriented_surface.hpp"
#include "tri_surface_search.hpp"
#include "ofstream.hpp"
#include "tree_bound_box.hpp"
#include "mesh_tools.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(booleanSurface, 0);
}
// Private Member Functions 
// Check whether at least one of faces connected to the intersection has been
// marked.
void mousse::booleanSurface::checkIncluded
(
  const intersectedSurface& surf,
  const labelList& faceZone,
  const label includedFace
)
{
  FOR_ALL(surf.intersectionEdges(), intEdgeI)
  {
    label edgeI = surf.intersectionEdges()[intEdgeI];
    const labelList& myFaces = surf.edgeFaces()[edgeI];
    bool usesIncluded = false;
    FOR_ALL(myFaces, myFaceI)
    {
      if (faceZone[myFaces[myFaceI]] == faceZone[includedFace])
      {
        usesIncluded = true;
        break;
      }
    }
    if (!usesIncluded)
    {
      FATAL_ERROR_IN
      (
        "booleanSurface::checkIncluded(const intersectedSurface&"
        ", const labelList&, const label)"
      )   << "None of the faces reachable from face " << includedFace
        << " connects to the intersection."
        << exit(FatalError);
    }
  }
}
// Linear lookup
mousse::label mousse::booleanSurface::index
(
  const labelList& elems,
  const label elem
)
{
  FOR_ALL(elems, elemI)
  {
    if (elems[elemI] == elem)
    {
      return elemI;
    }
  }
  return -1;
}
mousse::label mousse::booleanSurface::findEdge
(
  const edgeList& edges,
  const labelList& edgeLabels,
  const edge& e
)
{
  FOR_ALL(edgeLabels, edgeLabelI)
  {
    if (edges[edgeLabels[edgeLabelI]] == e)
    {
      return edgeLabels[edgeLabelI];
    }
  }
  FATAL_ERROR_IN
  (
    "booleanSurface::findEdge(const edgeList&, const labelList&"
    ", const edge&)"
  )   << "Cannot find edge " << e << " in edges " << edgeLabels
    << abort(FatalError);
  return -1;
}
// Generate combined patchList (returned). Sets patchMap to map from surf2
// region numbers into combined patchList
mousse::geometricSurfacePatchList mousse::booleanSurface::mergePatches
(
  const triSurface& surf1,
  const triSurface& surf2,
  labelList& patchMap2
)
{
  // Size too big.
  geometricSurfacePatchList combinedPatches
  (
    surf1.patches().size()
   + surf2.patches().size()
  );
  // Copy all patches of surf1
  label combinedPatchI = 0;
  FOR_ALL(surf1.patches(), patchI)
  {
    combinedPatches[combinedPatchI++] = surf1.patches()[patchI];
  }
  // (inefficiently) add unique patches from surf2
  patchMap2.setSize(surf2.patches().size());
  FOR_ALL(surf2.patches(), patch2I)
  {
    label index = -1;
    FOR_ALL(surf1.patches(), patch1I)
    {
      if (surf1.patches()[patch1I] == surf2.patches()[patch2I])
      {
        index = patch1I;
        break;
      }
    }
    if (index == -1)
    {
      combinedPatches[combinedPatchI] = surf2.patches()[patch2I];
      patchMap2[patch2I] = combinedPatchI;
      combinedPatchI++;
    }
    else
    {
      patchMap2[patch2I] = index;
    }
  }
  combinedPatches.setSize(combinedPatchI);
  return combinedPatches;
}
void mousse::booleanSurface::propagateEdgeSide
(
  const triSurface& surf,
  const label prevVert0,
  const label prevFaceI,
  const label prevState,
  const label edgeI,
  labelList& side
)
{
  const labelList& eFaces = surf.sortedEdgeFaces()[edgeI];
  // Simple case. Propagate side.
  if (eFaces.size() == 2)
  {
    FOR_ALL(eFaces, eFaceI)
    {
      propagateSide
      (
        surf,
        prevState,
        eFaces[eFaceI],
        side
      );
    }
  }
  if (((eFaces.size() % 2) == 1) && (eFaces.size() != 1))
  {
    FATAL_ERROR_IN
    (
      "booleanSurface::propagateEdgeSide(const triSurface&,"
      "const label, const label, const label, const label,"
      " labelList&)"
    )   << "Don't know how to handle edges with odd number of faces"
      << endl
      << "edge:" << edgeI << " vertices:" << surf.edges()[edgeI]
      << " coming from face:" << prevFaceI
      << " edgeFaces:" << eFaces << abort(FatalError);
  }
  // Get position of face in edgeFaces
  label ind = index(eFaces, prevFaceI);
  // Determine orientation of faces around edge prevVert0
  // (might be opposite of edge)
  const edge& e = surf.edges()[edgeI];
  // Get next face to include
  label nextInd;
  label prevInd;
  if (e.start() == prevVert0)
  {
    // Edge (and hence eFaces) in same order as prevVert0.
    // Take next face from sorted list
    nextInd = eFaces.fcIndex(ind);
    prevInd = eFaces.rcIndex(ind);
  }
  else
  {
    // Take previous face from sorted neighbours
    nextInd = eFaces.rcIndex(ind);
    prevInd = eFaces.fcIndex(ind);
  }
  if (prevState == OUTSIDE)
  {
    // Coming from outside. nextInd is outside, rest is inside.
    FOR_ALL(eFaces, eFaceI)
    {
      if (eFaceI != ind)
      {
        label nextState;
        if (eFaceI == nextInd)
        {
          nextState = OUTSIDE;
        }
        else
        {
          nextState = INSIDE;
        }
        propagateSide
        (
          surf,
          nextState,
          eFaces[eFaceI],
          side
        );
      }
    }
  }
  else
  {
    // Coming from inside. prevInd is inside as well, rest is outside.
    FOR_ALL(eFaces, eFaceI)
    {
      if (eFaceI != ind)
      {
        label nextState;
        if (eFaceI == prevInd)
        {
          nextState = INSIDE;
        }
        else
        {
          nextState = OUTSIDE;
        }
        propagateSide
        (
          surf,
          nextState,
          eFaces[eFaceI],
          side
        );
      }
    }
  }
}
// Face-edge walk. Determines inside/outside for all faces connected to an edge.
void mousse::booleanSurface::propagateSide
(
  const triSurface& surf,
  const label prevState,
  const label faceI,
  labelList& side
)
{
  if (side[faceI] == UNVISITED)
  {
    side[faceI] = prevState;
    const labelledTri& tri = surf.localFaces()[faceI];
    // Get copy of face labels
    label a = tri[0];
    label b = tri[1];
    label c = tri[2];
    // Go and visit my edges' face-neighbours.
    const labelList& myEdges = surf.faceEdges()[faceI];
    label edgeAB = findEdge(surf.edges(), myEdges, edge(a, b));
    propagateEdgeSide
    (
      surf,
      a,
      faceI,
      prevState,
      edgeAB,
      side
    );
    label edgeBC = findEdge(surf.edges(), myEdges, edge(b, c));
    propagateEdgeSide
    (
      surf,
      b,
      faceI,
      prevState,
      edgeBC,
      side
    );
    label edgeCA = findEdge(surf.edges(), myEdges, edge(c, a));
    propagateEdgeSide
    (
      surf,
      c,
      faceI,
      prevState,
      edgeCA,
      side
    );
  }
}
// Constructors 
// Null constructor
mousse::booleanSurface::booleanSurface()
:
  triSurface()
{}
// Construct from surfaces and face to include for every surface
mousse::booleanSurface::booleanSurface
(
  const triSurface& surf1,
  const triSurface& surf2,
  const surfaceIntersection& inter,
  const label includeFace1,
  const label includeFace2
)
:
  triSurface(),
  faceMap_()
{
  if (debug)
  {
    Pout<< "booleanSurface : Generating intersected surface for surf1"
      << endl;
  }
  // Add intersection to surface1 (retriangulates cut faces)
  intersectedSurface cutSurf1(surf1, true, inter);
  if (debug)
  {
    Pout<< "booleanSurface : Generated cutSurf1: " << endl;
    cutSurf1.writeStats(Pout);
    Pout<< "Writing to file cutSurf1.obj" << endl;
    cutSurf1.write("cutSurf1.obj");
  }
  if (debug)
  {
    Pout<< "booleanSurface : Generating intersected surface for surf2"
      << endl;
  }
  // Add intersection to surface2
  intersectedSurface cutSurf2(surf2, false, inter);
  if (debug)
  {
    Pout<< "booleanSurface : Generated cutSurf2: " << endl;
    cutSurf2.writeStats(Pout);
    Pout<< "Writing to file cutSurf2.obj" << endl;
    cutSurf2.write("cutSurf2.obj");
  }
  // Find (first) face of cutSurf1 that originates from includeFace1
  label cutSurf1FaceI = index(cutSurf1.faceMap(), includeFace1);
  if (debug)
  {
    Pout<< "cutSurf1 : starting to fill from face:" << cutSurf1FaceI
      << endl;
  }
  if (cutSurf1FaceI == -1)
  {
    FATAL_ERROR_IN
    (
     "booleanSurface(const triSurfaceSearch&"
      ", const label, const triSurfaceSearch&, const label)"
    )   << "Did not find face with label " << includeFace1
      << " in intersectedSurface."
      << exit(FatalError);
  }
  // Find (first) face of cutSurf2 that originates from includeFace1
  label cutSurf2FaceI = index(cutSurf2.faceMap(), includeFace2);
  if (debug)
  {
    Pout<< "cutSurf2 : starting to fill from face:" << cutSurf2FaceI
      << endl;
  }
  if (cutSurf2FaceI == -1)
  {
    FATAL_ERROR_IN
    (
     "booleanSurface(const triSurfaceSearch&"
      ", const label, const triSurfaceSearch&, const label)"
    )   << "Did not find face with label " << includeFace2
      << " in intersectedSurface."
      << exit(FatalError);
  }
  //
  // Mark faces of cutSurf1 that need to be kept by walking from includeFace1
  // without crossing any edges of the intersection.
  //
  // Mark edges on intersection
  const labelList& int1Edges = cutSurf1.intersectionEdges();
  boolList isIntersectionEdge1(cutSurf1.nEdges(), false);
  FOR_ALL(int1Edges, intEdgeI)
  {
    label edgeI = int1Edges[intEdgeI];
    isIntersectionEdge1[edgeI] = true;
  }
  labelList faceZone1;
  cutSurf1.markZones(isIntersectionEdge1, faceZone1);
  // Check whether at least one of sides of intersection has been marked.
  checkIncluded(cutSurf1, faceZone1, cutSurf1FaceI);
  // Subset zone which includes cutSurf2FaceI
  boolList includedFaces1(cutSurf1.size(), false);
  FOR_ALL(faceZone1, faceI)
  {
    if (faceZone1[faceI] == faceZone1[cutSurf1FaceI])
    {
      includedFaces1[faceI] = true;
    }
  }
  // Subset to include only interesting part
  labelList pointMap1;
  labelList faceMap1;
  triSurface subSurf1
  (
    cutSurf1.subsetMesh
    (
      includedFaces1,
      pointMap1,
      faceMap1
    )
  );
  //
  // Mark faces of cutSurf2 that need to be kept by walking from includeFace2
  // without crossing any edges of the intersection.
  //
  // Mark edges and points on intersection
  const labelList& int2Edges = cutSurf2.intersectionEdges();
  boolList isIntersectionEdge2(cutSurf2.nEdges(), false);
  FOR_ALL(int2Edges, intEdgeI)
  {
    label edgeI = int2Edges[intEdgeI];
    isIntersectionEdge2[edgeI] = true;
  }
  labelList faceZone2;
  cutSurf2.markZones(isIntersectionEdge2, faceZone2);
  // Check whether at least one of sides of intersection has been marked.
  checkIncluded(cutSurf2, faceZone2, cutSurf2FaceI);
  // Subset zone which includes cutSurf2FaceI
  boolList includedFaces2(cutSurf2.size(), false);
  FOR_ALL(faceZone2, faceI)
  {
    if (faceZone2[faceI] == faceZone2[cutSurf2FaceI])
    {
      includedFaces2[faceI] = true;
    }
  }
  labelList pointMap2;
  labelList faceMap2;
  triSurface subSurf2
  (
    cutSurf2.subsetMesh
    (
      includedFaces2,
      pointMap2,
      faceMap2
    )
  );
  //
  // Now match up the corresponding points on the intersection. The
  // intersectedSurfaces will have the points resulting from the
  // intersection last in their points and in the same
  // order so we can use the pointMaps from the subsets to find them.
  //
  // We keep the vertices on the first surface and renumber those on the
  // second one.
  //
  // points
  //
  pointField combinedPoints
  (
    subSurf1.nPoints()
   + subSurf2.nPoints()
   - (cutSurf2.nPoints() - cutSurf2.nSurfacePoints())
  );
  // Copy points from subSurf1 and remember the labels of the ones in
  // the intersection
  labelList intersectionLabels
  (
    cutSurf1.nPoints() - cutSurf1.nSurfacePoints()
  );
  label combinedPointI = 0;
  FOR_ALL(subSurf1.points(), pointI)
  {
    // Label in cutSurf
    label cutSurfPointI = pointMap1[pointI];
    if (!cutSurf1.isSurfacePoint(cutSurfPointI))
    {
      // Label in original intersection is equal to the cutSurfPointI
      // Remember label in combinedPoints for intersection point.
      intersectionLabels[cutSurfPointI] = combinedPointI;
    }
    // Copy point
    combinedPoints[combinedPointI++] = subSurf1.points()[pointI];
  }
  // Append points from subSurf2 (if they are not intersection points)
  // and construct mapping
  labelList pointMap(subSurf2.nPoints());
  FOR_ALL(subSurf2.points(), pointI)
  {
    // Label in cutSurf
    label cutSurfPointI = pointMap2[pointI];
    if (!cutSurf2.isSurfacePoint(cutSurfPointI))
    {
      // Lookup its label in combined point list.
      pointMap[pointI] = intersectionLabels[cutSurfPointI];
    }
    else
    {
      pointMap[pointI] = combinedPointI;
      combinedPoints[combinedPointI++] = subSurf2.points()[pointI];
    }
  }
  //
  // patches
  //
  labelList patchMap2;
  geometricSurfacePatchList combinedPatches
  (
    mergePatches
    (
      surf1,
      surf2,
      patchMap2
    )
  );
  //
  // faces
  //
  List<labelledTri> combinedFaces(subSurf1.size() + subSurf2.size());
  faceMap_.setSize(combinedFaces.size());
  // Copy faces from subSurf1. No need for renumbering.
  label combinedFaceI = 0;
  FOR_ALL(subSurf1, faceI)
  {
    faceMap_[combinedFaceI] = faceMap1[faceI];
    combinedFaces[combinedFaceI++] = subSurf1[faceI];
  }
  // Copy and renumber faces from subSurf2.
  FOR_ALL(subSurf2, faceI)
  {
    const labelledTri& f = subSurf2[faceI];
    faceMap_[combinedFaceI] = -faceMap2[faceI]-1;
    combinedFaces[combinedFaceI++] =
      labelledTri
      (
        pointMap[f[0]],
        pointMap[f[1]],
        pointMap[f[2]],
        patchMap2[f.region()]
      );
  }
  triSurface::operator=
  (
    triSurface
    (
      combinedFaces,
      combinedPatches,
      combinedPoints
    )
  );
}
// Construct from surfaces and boolean operation
mousse::booleanSurface::booleanSurface
(
  const triSurface& surf1,
  const triSurface& surf2,
  const surfaceIntersection& inter,
  const label booleanOp
)
:
  triSurface(),
  faceMap_()
{
  if (debug)
  {
    Pout<< "booleanSurface : Testing surf1 and surf2" << endl;
    {
      const labelListList& edgeFaces = surf1.edgeFaces();
      FOR_ALL(edgeFaces, edgeI)
      {
        const labelList& eFaces = edgeFaces[edgeI];
        if (eFaces.size() == 1)
        {
          WARNING_IN("booleanSurface::booleanSurface")
            << "surf1 is open surface at edge " << edgeI
            << " verts:" << surf1.edges()[edgeI]
            << " connected to faces " << eFaces << endl;
        }
      }
    }
    {
      const labelListList& edgeFaces = surf2.edgeFaces();
      FOR_ALL(edgeFaces, edgeI)
      {
        const labelList& eFaces = edgeFaces[edgeI];
        if (eFaces.size() == 1)
        {
          WARNING_IN("booleanSurface::booleanSurface")
            << "surf2 is open surface at edge " << edgeI
            << " verts:" << surf2.edges()[edgeI]
            << " connected to faces " << eFaces << endl;
        }
      }
    }
  }
  //
  // Surface 1
  //
  if (debug)
  {
    Pout<< "booleanSurface : Generating intersected surface for surf1"
      << endl;
  }
  // Add intersection to surface1 (retriangulates cut faces)
  intersectedSurface cutSurf1(surf1, true, inter);
  if (debug)
  {
    Pout<< "booleanSurface : Generated cutSurf1: " << endl;
    cutSurf1.writeStats(Pout);
    Pout<< "Writing to file cutSurf1.obj" << endl;
    cutSurf1.write("cutSurf1.obj");
  }
  //
  // Surface 2
  //
  if (debug)
  {
    Pout<< "booleanSurface : Generating intersected surface for surf2"
      << endl;
  }
  // Add intersection to surface2
  intersectedSurface cutSurf2(surf2, false, inter);
  if (debug)
  {
    Pout<< "booleanSurface : Generated cutSurf2: " << endl;
    cutSurf2.writeStats(Pout);
    Pout<< "Writing to file cutSurf2.obj" << endl;
    cutSurf2.write("cutSurf2.obj");
  }
  //
  // patches
  //
  labelList patchMap2;
  geometricSurfacePatchList combinedPatches
  (
    mergePatches
    (
      surf1,
      surf2,
      patchMap2
    )
  );
  //
  // Now match up the corresponding points on the intersection. The
  // intersectedSurfaces will have the points resulting from the
  // intersection first in their points and in the same
  // order
  //
  // We keep the vertices on the first surface and renumber those on the
  // second one.
  pointField combinedPoints(cutSurf1.nPoints() + cutSurf2.nSurfacePoints());
  // Copy all points from 1 and non-intersection ones from 2.
  label combinedPointI = 0;
  FOR_ALL(cutSurf1.points(), pointI)
  {
    combinedPoints[combinedPointI++] = cutSurf1.points()[pointI];
  }
  for
  (
    label pointI = 0;
    pointI < cutSurf2.nSurfacePoints();
    pointI++
  )
  {
    combinedPoints[combinedPointI++] = cutSurf2.points()[pointI];
  }
  // Point order is now
  // - 0.. cutSurf1.nSurfacePoints : original surf1 points
  // -  .. cutSurf1.nPoints        : intersection points
  // -  .. cutSurf2.nSurfacePoints : original surf2 points
  if (debug)
  {
    Pout<< "booleanSurface : generated points:" << nl
      << "    0 .. " << cutSurf1.nSurfacePoints()-1
      << " : original surface1"
      << nl
      << "    " << cutSurf1.nSurfacePoints()
      << " .. " << cutSurf1.nPoints()-1
      << " : intersection points"
      << nl
      << "    " << cutSurf1.nPoints() << " .. "
      << cutSurf2.nSurfacePoints()-1
      << " : surface2 points"
      << nl
      << endl;
  }
  // Copy faces. Faces from surface 1 keep vertex numbering and region info.
  // Faces from 2 get vertices and region renumbered.
  List<labelledTri> combinedFaces(cutSurf1.size() + cutSurf2.size());
  label combinedFaceI = 0;
  FOR_ALL(cutSurf1, faceI)
  {
    combinedFaces[combinedFaceI++] = cutSurf1[faceI];
  }
  FOR_ALL(cutSurf2, faceI)
  {
    labelledTri& combinedTri = combinedFaces[combinedFaceI++];
    const labelledTri& tri = cutSurf2[faceI];
    FOR_ALL(tri, fp)
    {
      if (cutSurf2.isSurfacePoint(tri[fp]))
      {
        // Renumber. Surface2 points are after ones from surf 1.
        combinedTri[fp] = tri[fp] + cutSurf1.nPoints();
      }
      else
      {
        // Is intersection.
        combinedTri[fp] =
          tri[fp]
         - cutSurf2.nSurfacePoints()
         + cutSurf1.nSurfacePoints();
      }
    }
    combinedTri.region() = patchMap2[tri.region()];
  }
  // Now we have surface in combinedFaces and combinedPoints. Use
  // booleanOp to determine which part of what to keep.
  // Construct addressing for whole part.
  triSurface combinedSurf
  (
    combinedFaces,
    combinedPatches,
    combinedPoints
  );
  if (debug)
  {
    Pout<< "booleanSurface : Generated combinedSurf: " << endl;
    combinedSurf.writeStats(Pout);
    Pout<< "Writing to file combinedSurf.obj" << endl;
    combinedSurf.write("combinedSurf.obj");
  }
  if (booleanOp == booleanSurface::ALL)
  {
    // Special case: leave surface multiply connected
    faceMap_.setSize(combinedSurf.size());
    label combinedFaceI = 0;
    FOR_ALL(cutSurf1, faceI)
    {
      faceMap_[combinedFaceI++] = cutSurf1.faceMap()[faceI];
    }
    FOR_ALL(cutSurf2, faceI)
    {
      faceMap_[combinedFaceI++] = -cutSurf2.faceMap()[faceI] - 1;
    }
    triSurface::operator=(combinedSurf);
    return;
  }
  // Get outside point.
  point outsidePoint = 2 * treeBoundBox(combinedSurf.localPoints()).span();
  //
  // Linear search for nearest point on surface.
  //
  const pointField& pts = combinedSurf.points();
  label minFaceI = -1;
  pointHit minHit(false, vector::zero, GREAT, true);
  FOR_ALL(combinedSurf, faceI)
  {
    pointHit curHit = combinedSurf[faceI].nearestPoint(outsidePoint, pts);
    if (curHit.distance() < minHit.distance())
    {
      minHit = curHit;
      minFaceI = faceI;
    }
  }
  if (debug)
  {
    Pout<< "booleanSurface : found for point:" << outsidePoint
      << "  nearest face:" << minFaceI
      << "  nearest point:" << minHit.rawPoint()
      << endl;
  }
  // Visibility/side of face:
  //      UNVISITED: unvisited
  //      OUTSIDE: visible from outside
  //      INSIDE: invisible from outside
  labelList side(combinedSurf.size(), UNVISITED);
  // Walk face-edge-face and propagate inside/outside status.
  propagateSide(combinedSurf, OUTSIDE, minFaceI, side);
  // Depending on operation include certain faces.
  //  INTERSECTION: faces on inside of 1 and of 2
  //  UNION: faces on outside of 1 and of 2
  //  DIFFERENCE: faces on outside of 1 and inside of 2
  boolList include(combinedSurf.size(), false);
  FOR_ALL(side, faceI)
  {
    if (side[faceI] == UNVISITED)
    {
      FATAL_ERROR_IN
      (
        "booleanSurface::booleanSurface"
        "(const triSurfaceSearch&, const triSurfaceSearch&"
        ", const label booleanOp)"
      )   << "Face " << faceI << " has not been reached by walking from"
        << " nearest point " << minHit.rawPoint()
        << " nearest face " << minFaceI << exit(FatalError);
    }
    else if (side[faceI] == OUTSIDE)
    {
      if (booleanOp == booleanSurface::UNION)
      {
        include[faceI] = true;
      }
      else if (booleanOp == booleanSurface::INTERSECTION)
      {
        include[faceI] = false;
      }
      else    // difference
      {
        include[faceI] = (faceI < cutSurf1.size()); // face from surf1
      }
    }
    else    // inside
    {
      if (booleanOp == booleanSurface::UNION)
      {
        include[faceI] = false;
      }
      else if (booleanOp == booleanSurface::INTERSECTION)
      {
        include[faceI] = true;
      }
      else    // difference
      {
        include[faceI] = (faceI >= cutSurf1.size()); // face from surf2
      }
    }
  }
  // Create subsetted surface
  labelList subToCombinedPoint;
  labelList subToCombinedFace;
  triSurface subSurf
  (
    combinedSurf.subsetMesh
    (
      include,
      subToCombinedPoint,
      subToCombinedFace
    )
  );
  // Create face map
  faceMap_.setSize(subSurf.size());
  FOR_ALL(subToCombinedFace, faceI)
  {
    // Get label in combinedSurf
    label combinedFaceI = subToCombinedFace[faceI];
    // First faces in combinedSurf come from cutSurf1.
    if (combinedFaceI < cutSurf1.size())
    {
      label cutSurf1Face = combinedFaceI;
      faceMap_[faceI] = cutSurf1.faceMap()[cutSurf1Face];
    }
    else
    {
      label cutSurf2Face = combinedFaceI - cutSurf1.size();
      faceMap_[faceI] = - cutSurf2.faceMap()[cutSurf2Face] - 1;
    }
  }
  // Orient outwards
  orientedSurface outSurf(subSurf);
  // Assign.
  triSurface::operator=(outSurf);
}
