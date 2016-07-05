// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_intersection.hpp"
#include "tri_surface_search.hpp"
#include "label_pair_lookup.hpp"
#include "ofstream.hpp"
#include "hash_set.hpp"
#include "tri_surface.hpp"
#include "point_index_hit.hpp"
#include "mesh_tools.hpp"


// Private Member Functions 
void mousse::surfaceIntersection::writeOBJ(const point& pt, Ostream& os)
{
  os << "v " << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
}


void mousse::surfaceIntersection::writeOBJ
(
  const List<point>& pts,
  const List<edge>& edges,
  Ostream& os
)
{
  FOR_ALL(pts, i) {
    writeOBJ(pts[i], os);
  }
  FOR_ALL(edges, i) {
    const edge& e = edges[i];
    os << "l " << e.start()+1 << ' ' << e.end()+1 << endl;
  }
}


// Get minimum length of all edges connected to point
mousse::scalar mousse::surfaceIntersection::minEdgeLen
(
  const triSurface& surf,
  const label pointI
)
{
  const labelList& pEdges = surf.pointEdges()[pointI];
  scalar minLen = GREAT;
  FOR_ALL(pEdges, pEdgeI) {
    const edge& e = surf.edges()[pEdges[pEdgeI]];
    minLen = min(minLen, e.mag(surf.localPoints()));
  }
  return minLen;
}


// Get edge between fp and fp+1 on faceI.
mousse::label mousse::surfaceIntersection::getEdge
(
  const triSurface& surf,
  const label faceI,
  const label fp
)
{
  const edge faceEdge = surf.localFaces()[faceI].faceEdge(fp);
  const labelList& eLabels = surf.faceEdges()[faceI];
  FOR_ALL(eLabels, eI) {
    const label edgeI = eLabels[eI];
    if (surf.edges()[edgeI] == faceEdge) {
      return edgeI;
    }
  }
  FATAL_ERROR_IN
  (
    "surfaceIntersection::getEdge(const triSurface&"
    ", const label, const label"
  )
  << "Problem:: Cannot find edge with vertices " << faceEdge
  << " in face " << faceI
  << abort(FatalError);
  return -1;
}


// Given a map remove all consecutive duplicate elements.
void mousse::surfaceIntersection::removeDuplicates
(
  const labelList& map,
  labelList& elems
)
{
  bool hasDuplicate = false;
  label prevVertI = -1;
  FOR_ALL(elems, elemI) {
    label newVertI = map[elems[elemI]];
    if (newVertI == prevVertI) {
      hasDuplicate = true;
      break;
    }
    prevVertI = newVertI;
  }
  if (hasDuplicate) {
    // Create copy
    labelList oldElems(elems);
    label elemI = 0;
    // Insert first
    elems[elemI++] = map[oldElems[0]];
    for (label vertI = 1; vertI < oldElems.size(); vertI++) {
      // Insert others only if they differ from one before
      label newVertI = map[oldElems[vertI]];
      if (newVertI != elems.last()) {
        elems[elemI++] = newVertI;
      }
    }
    elems.setSize(elemI);
  }
}


// Remap.
void mousse::surfaceIntersection::inlineRemap
(
  const labelList& map,
  labelList& elems
)
{
  FOR_ALL(elems, elemI) {
    elems[elemI] = map[elems[elemI]];
  }
}


// Remove all duplicate and degenerate elements. Return unique elements and
// map from old to new.
mousse::edgeList mousse::surfaceIntersection::filterEdges
(
  const edgeList& edges,
  labelList& map
)
{
  HashSet<edge, Hash<edge>> uniqueEdges{10*edges.size()};
  edgeList newEdges{edges.size()};
  map.setSize(edges.size());
  map = -1;
  label newEdgeI = 0;
  FOR_ALL(edges, edgeI) {
    const edge& e = edges[edgeI];
    if ((e.start() != e.end()) && (uniqueEdges.find(e) == uniqueEdges.end())) {
      // Edge is -non degenerate and -not yet seen.
      uniqueEdges.insert(e);
      map[edgeI] = newEdgeI;
      newEdges[newEdgeI++] = e;
    }
  }
  newEdges.setSize(newEdgeI);
  return newEdges;
}


// Remove all duplicate elements.
mousse::labelList mousse::surfaceIntersection::filterLabels
(
  const labelList& elems,
  labelList& map
)
{
  labelHashSet uniqueElems{10*elems.size()};
  labelList newElems{elems.size()};
  map.setSize(elems.size());
  map = -1;
  label newElemI = 0;
  FOR_ALL(elems, elemI) {
    label elem = elems[elemI];
    if (uniqueElems.find(elem) == uniqueElems.end()) {
      // First time elem is seen
      uniqueElems.insert(elem);
      map[elemI] = newElemI;
      newElems[newElemI++] = elem;
    }
  }
  newElems.setSize(newElemI);
  return newElems;
}


void mousse::surfaceIntersection::writeIntersectedEdges
(
  const triSurface& surf,
  const labelListList& edgeCutVerts,
  Ostream& os
) const
{
  // Dump all points (surface followed by cutPoints)
  const pointField& pts = surf.localPoints();
  FOR_ALL(pts, pointI) {
    writeOBJ(pts[pointI], os);
  }
  FOR_ALL(cutPoints(), cutPointI) {
    writeOBJ(cutPoints()[cutPointI], os);
  }
  FOR_ALL(edgeCutVerts, edgeI) {
    const labelList& extraVerts = edgeCutVerts[edgeI];
    if (extraVerts.size()) {
      const edge& e = surf.edges()[edgeI];
      // Start of original edge to first extra point
      os << "l " << e.start()+1 << ' '
        << extraVerts[0] + surf.nPoints() + 1 << endl;
      for (label i = 1; i < extraVerts.size(); i++) {
        os << "l " << extraVerts[i-1] + surf.nPoints() + 1  << ' '
          << extraVerts[i] + surf.nPoints() + 1 << endl;
      }
      os << "l " << extraVerts.last() + surf.nPoints() + 1
        << ' ' << e.end()+1 << endl;
    }
  }
}


// Return 0 (p close to start), 1(close to end) or -1.
mousse::label mousse::surfaceIntersection::classify
(
  const scalar startTol,
  const scalar endTol,
  const point& p,
  const edge& e,
  const pointField& points
)
{
  if (mag(p - points[e.start()]) < startTol) {
    return 0;
  } else if (mag(p - points[e.end()]) < endTol) {
    return 1;
  } else {
    return -1;
  }
}

