// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_search.hpp"
#include "poly_mesh.hpp"
#include "indexed_octree.hpp"
#include "dynamic_list.hpp"
#include "demand_driven_data.hpp"
#include "tree_data_cell.hpp"
#include "tree_data_face.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(meshSearch, 0);
scalar meshSearch::tol_ = 1e-3;

}


// Private Member Functions 
bool mousse::meshSearch::findNearer
(
  const point& sample,
  const pointField& points,
  label& nearestI,
  scalar& nearestDistSqr
)
{
  bool nearer = false;
  FOR_ALL(points, pointI) {
    scalar distSqr = magSqr(points[pointI] - sample);
    if (distSqr < nearestDistSqr) {
      nearestDistSqr = distSqr;
      nearestI = pointI;
      nearer = true;
    }
  }
  return nearer;
}


bool mousse::meshSearch::findNearer
(
  const point& sample,
  const pointField& points,
  const labelList& indices,
  label& nearestI,
  scalar& nearestDistSqr
)
{
  bool nearer = false;
  FOR_ALL(indices, i) {
    label pointI = indices[i];
    scalar distSqr = magSqr(points[pointI] - sample);
    if (distSqr < nearestDistSqr) {
      nearestDistSqr = distSqr;
      nearestI = pointI;
      nearer = true;
    }
  }
  return nearer;
}


// tree based searching
mousse::label mousse::meshSearch::findNearestCellTree(const point& location) const
{
  const indexedOctree<treeDataCell>& tree = cellTree();
  pointIndexHit info = tree.findNearest
  (
    location,
    magSqr(tree.bb().max()-tree.bb().min())
  );
  if (!info.hit()) {
    info = tree.findNearest(location, mousse::sqr(GREAT));
  }
  return info.index();
}


// linear searching
mousse::label mousse::meshSearch::findNearestCellLinear(const point& location) const
{
  const vectorField& centres = mesh_.cellCentres();
  label nearestIndex = 0;
  scalar minProximity = magSqr(centres[nearestIndex] - location);
  findNearer
  (
    location,
    centres,
    nearestIndex,
    minProximity
  );
  return nearestIndex;
}


// walking from seed
mousse::label mousse::meshSearch::findNearestCellWalk
(
  const point& location,
  const label seedCellI
) const
{
  if (seedCellI < 0) {
    FATAL_ERROR_IN
    (
      "meshSearch::findNearestCellWalk(const point&, const label)"
    )
    << "illegal seedCell:" << seedCellI << exit(FatalError);
  }
  // Walk in direction of face that decreases distance
  label curCellI = seedCellI;
  scalar distanceSqr = magSqr(mesh_.cellCentres()[curCellI] - location);
  bool closer;
  do {
    // Try neighbours of curCellI
    closer = findNearer
    (
      location,
      mesh_.cellCentres(),
      mesh_.cellCells()[curCellI],
      curCellI,
      distanceSqr
    );
  } while (closer);
  return curCellI;
}


// tree based searching
mousse::label mousse::meshSearch::findNearestFaceTree(const point& location) const
{
  // Search nearest cell centre.
  const indexedOctree<treeDataCell>& tree = cellTree();
  // Search with decent span
  pointIndexHit info = tree.findNearest
  (
    location,
    magSqr(tree.bb().max()-tree.bb().min())
  );
  if (!info.hit()) {
    // Search with desparate span
    info = tree.findNearest(location, mousse::sqr(GREAT));
  }
  // Now check any of the faces of the nearest cell
  const vectorField& centres = mesh_.faceCentres();
  const cell& ownFaces = mesh_.cells()[info.index()];
  label nearestFaceI = ownFaces[0];
  scalar minProximity = magSqr(centres[nearestFaceI] - location);
  findNearer
  (
    location,
    centres,
    ownFaces,
    nearestFaceI,
    minProximity
  );
  return nearestFaceI;
}


// linear searching
mousse::label mousse::meshSearch::findNearestFaceLinear(const point& location) const
{
  const vectorField& centres = mesh_.faceCentres();
  label nearestFaceI = 0;
  scalar minProximity = magSqr(centres[nearestFaceI] - location);
  findNearer
  (
    location,
    centres,
    nearestFaceI,
    minProximity
  );
  return nearestFaceI;
}


// walking from seed
mousse::label mousse::meshSearch::findNearestFaceWalk
(
  const point& location,
  const label seedFaceI
) const
{
  if (seedFaceI < 0) {
    FATAL_ERROR_IN
    (
      "meshSearch::findNearestFaceWalk(const point&, const label)"
    )
    << "illegal seedFace:" << seedFaceI << exit(FatalError);
  }
  const vectorField& centres = mesh_.faceCentres();
  // Walk in direction of face that decreases distance
  label curFaceI = seedFaceI;
  scalar distanceSqr = magSqr(centres[curFaceI] - location);
  while (true) {
    label betterFaceI = curFaceI;
    findNearer
    (
      location,
      centres,
      mesh_.cells()[mesh_.faceOwner()[curFaceI]],
      betterFaceI,
      distanceSqr
    );
    if (mesh_.isInternalFace(curFaceI)) {
      findNearer
      (
        location,
        centres,
        mesh_.cells()[mesh_.faceNeighbour()[curFaceI]],
        betterFaceI,
        distanceSqr
      );
    }
    if (betterFaceI == curFaceI) {
      break;
    }
    curFaceI = betterFaceI;
  }
  return curFaceI;
}


mousse::label mousse::meshSearch::findCellLinear(const point& location) const
{
  label n = 0;
  while (n < mesh_.nCells()) {
    if (mesh_.pointInCell(location, n, cellDecompMode_)) {
      return n;
    }
    n++;
  }
  return -1;
}


// walking from seed
mousse::label mousse::meshSearch::findCellWalk
(
  const point& location,
  const label seedCellI
) const
{
  if (seedCellI < 0) {
    FATAL_ERROR_IN
    (
      "meshSearch::findCellWalk(const point&, const label)"
    )
    << "illegal seedCell:" << seedCellI << exit(FatalError);
  }
  if (mesh_.pointInCell(location, seedCellI, cellDecompMode_)) {
    return seedCellI;
  }
  // Walk in direction of face that decreases distance
  label curCellI = seedCellI;
  scalar nearestDistSqr = magSqr(mesh_.cellCentres()[curCellI] - location);
  while(true) {
    // Try neighbours of curCellI
    const cell& cFaces = mesh_.cells()[curCellI];
    label nearestCellI = -1;
    FOR_ALL(cFaces, i) {
      label faceI = cFaces[i];
      if (!mesh_.isInternalFace(faceI))
        continue;
      label cellI = mesh_.faceOwner()[faceI];
      if (cellI == curCellI) {
        cellI = mesh_.faceNeighbour()[faceI];
      }
      // Check if this is the correct cell
      if (mesh_.pointInCell(location, cellI, cellDecompMode_)) {
        return cellI;
      }
      // Also calculate the nearest cell
      scalar distSqr = magSqr(mesh_.cellCentres()[cellI] - location);
      if (distSqr < nearestDistSqr) {
        nearestDistSqr = distSqr;
        nearestCellI = cellI;
      }
    }
    if (nearestCellI == -1) {
      return -1;
    }
    // Continue with the nearest cell
    curCellI = nearestCellI;
  }
  return -1;
}


mousse::label mousse::meshSearch::findNearestBoundaryFaceWalk
(
  const point& location,
  const label seedFaceI
) const
{
  if (seedFaceI < 0) {
    FATAL_ERROR_IN
    (
      "meshSearch::findNearestBoundaryFaceWalk"
      "(const point&, const label)"
    )
    << "illegal seedFace:" << seedFaceI << exit(FatalError);
  }
  // Start off from seedFaceI
  label curFaceI = seedFaceI;
  const face& f =  mesh_.faces()[curFaceI];
  scalar minDist = f.nearestPoint
  (
    location,
    mesh_.points()
  ).distance();
  bool closer;
  do {
    closer = false;
    // Search through all neighbouring boundary faces by going
    // across edges
    label lastFaceI = curFaceI;
    const labelList& myEdges = mesh_.faceEdges()[curFaceI];
    FOR_ALL(myEdges, myEdgeI) {
      const labelList& neighbours = mesh_.edgeFaces()[myEdges[myEdgeI]];
      // Check any face which uses edge, is boundary face and
      // is not curFaceI itself.
      FOR_ALL(neighbours, nI) {
        label faceI = neighbours[nI];
        if (faceI < mesh_.nInternalFaces() || faceI == lastFaceI)
          continue;
        const face& f =  mesh_.faces()[faceI];
        pointHit curHit = f.nearestPoint
        (
          location,
          mesh_.points()
        );
        // If the face is closer, reset current face and distance
        if (curHit.distance() < minDist) {
          minDist = curHit.distance();
          curFaceI = faceI;
          closer = true;  // a closer neighbour has been found
        }
      }
    }
  } while (closer);
  return curFaceI;
}


mousse::vector mousse::meshSearch::offset
(
  const point& bPoint,
  const label bFaceI,
  const vector& dir
) const
{
  // Get the neighbouring cell
  label ownerCellI = mesh_.faceOwner()[bFaceI];
  const point& c = mesh_.cellCentres()[ownerCellI];
  // Typical dimension: distance from point on face to cell centre
  scalar typDim = mag(c - bPoint);
  return tol_*typDim*dir;
}


// Constructors 
mousse::meshSearch::meshSearch
(
  const polyMesh& mesh,
  const polyMesh::cellDecomposition cellDecompMode
)
:
  mesh_{mesh},
  cellDecompMode_{cellDecompMode}
{
  if (cellDecompMode_ == polyMesh::FACE_DIAG_TRIS) {
    // Force construction of face diagonals
    (void)mesh.tetBasePtIs();
  }
}


// Construct with a custom bounding box
mousse::meshSearch::meshSearch
(
  const polyMesh& mesh,
  const treeBoundBox& bb,
  const polyMesh::cellDecomposition cellDecompMode
)
:
  mesh_{mesh},
  cellDecompMode_{cellDecompMode}
{
  overallBbPtr_.reset(new treeBoundBox{bb});
  if (cellDecompMode_ == polyMesh::FACE_DIAG_TRIS) {
    // Force construction of face diagonals
    (void)mesh.tetBasePtIs();
  }
}


// Destructor 
mousse::meshSearch::~meshSearch()
{
  clearOut();
}


// Member Functions 
const mousse::indexedOctree<mousse::treeDataFace>& mousse::meshSearch::boundaryTree()
const
{
  if (boundaryTreePtr_.valid())
    return boundaryTreePtr_();
  // Construct tree
  if (!overallBbPtr_.valid()) {
    Random rndGen{261782};
    overallBbPtr_.reset
    (
      new treeBoundBox{mesh_.points()}
    );
    treeBoundBox& overallBb = overallBbPtr_();
    // Extend slightly and make 3D
    overallBb = overallBb.extend(rndGen, 1e-4);
    overallBb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    overallBb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
  }
  // all boundary faces (not just walls)
  labelList bndFaces{mesh_.nFaces()-mesh_.nInternalFaces()};
  FOR_ALL(bndFaces, i) {
    bndFaces[i] = mesh_.nInternalFaces() + i;
  }
  boundaryTreePtr_.reset
  (
    new indexedOctree<treeDataFace>
    {
      treeDataFace    // all information needed to search faces
      {
        false,                      // do not cache bb
        mesh_,
        bndFaces                    // boundary faces only
      },
      overallBbPtr_(),                // overall search domain
      8,                              // maxLevel
      10,                             // leafsize
      3.0                             // duplicity
    }
  );
  return boundaryTreePtr_();
}


const mousse::indexedOctree<mousse::treeDataCell>& mousse::meshSearch::cellTree()
const
{
  if (cellTreePtr_.valid())
    return cellTreePtr_();
  // Construct tree
  if (!overallBbPtr_.valid()) {
    Random rndGen{261782};
    overallBbPtr_.reset
    (
      new treeBoundBox{mesh_.points()}
    );
    treeBoundBox& overallBb = overallBbPtr_();
    // Extend slightly and make 3D
    overallBb = overallBb.extend(rndGen, 1e-4);
    overallBb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
    overallBb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
  }
  cellTreePtr_.reset
  (
    new indexedOctree<treeDataCell>
    {
      treeDataCell
      {
        false,          // not cache bb
        mesh_,
        cellDecompMode_ // cell decomposition mode for inside tests
      },
      overallBbPtr_(),
      8,              // maxLevel
      10,             // leafsize
      6.0             // duplicity
    }
  );
  return cellTreePtr_();
}


mousse::label mousse::meshSearch::findNearestCell
(
  const point& location,
  const label seedCellI,
  const bool useTreeSearch
) const
{
  if (seedCellI == -1)
  {
    if (useTreeSearch) {
      return findNearestCellTree(location);
    } else {
      return findNearestCellLinear(location);
    }
  }
  return findNearestCellWalk(location, seedCellI);
}


mousse::label mousse::meshSearch::findNearestFace
(
  const point& location,
  const label seedFaceI,
  const bool useTreeSearch
) const
{
  if (seedFaceI == -1) {
    if (useTreeSearch) {
      return findNearestFaceTree(location);
    } else {
      return findNearestFaceLinear(location);
    }
  }
  return findNearestFaceWalk(location, seedFaceI);
}


mousse::label mousse::meshSearch::findCell
(
  const point& location,
  const label seedCellI,
  const bool useTreeSearch
) const
{
  // Find the nearest cell centre to this location
  if (seedCellI == -1) {
    if (useTreeSearch) {
      return cellTree().findInside(location);
    } else {
      return findCellLinear(location);
    }
  }
  return findCellWalk(location, seedCellI);
}


mousse::label mousse::meshSearch::findNearestBoundaryFace
(
  const point& location,
  const label seedFaceI,
  const bool useTreeSearch
) const
{
  if (seedFaceI == -1) {
    if (useTreeSearch) {
      const indexedOctree<treeDataFace>& tree = boundaryTree();
      pointIndexHit info = boundaryTree().findNearest
      (
        location,
        magSqr(tree.bb().max()-tree.bb().min())
      );
      if (!info.hit()) {
        info = boundaryTree().findNearest
        (
          location,
          mousse::sqr(GREAT)
        );
      }
      return tree.shapes().faceLabels()[info.index()];
    }
    scalar minDist = GREAT;
    label minFaceI = -1;
    for
    (
      label faceI = mesh_.nInternalFaces();
      faceI < mesh_.nFaces();
      faceI++
    ) {
      const face& f =  mesh_.faces()[faceI];
      pointHit curHit =
        f.nearestPoint
        (
          location,
          mesh_.points()
        );
      if (curHit.distance() < minDist) {
        minDist = curHit.distance();
        minFaceI = faceI;
      }
    }
    return minFaceI;
  }
  return findNearestBoundaryFaceWalk(location, seedFaceI);
}


mousse::pointIndexHit mousse::meshSearch::intersection
(
  const point& pStart,
  const point& pEnd
) const
{
  pointIndexHit curHit = boundaryTree().findLine(pStart, pEnd);
  if (curHit.hit()) {
    // Change index into octreeData into face label
    curHit.setIndex(boundaryTree().shapes().faceLabels()[curHit.index()]);
  }
  return curHit;
}


mousse::List<mousse::pointIndexHit> mousse::meshSearch::intersections
(
  const point& pStart,
  const point& pEnd
) const
{
  DynamicList<pointIndexHit> hits;
  vector edgeVec = pEnd - pStart;
  edgeVec /= mag(edgeVec);
  point pt = pStart;
  pointIndexHit bHit;
  do {
    bHit = intersection(pt, pEnd);
    if (bHit.hit()) {
      hits.append(bHit);
      const vector& area = mesh_.faceAreas()[bHit.index()];
      scalar typDim = mousse::sqrt(mag(area));
      if ((mag(bHit.hitPoint() - pEnd)/typDim) < SMALL) {
        break;
      }
      // Restart from hitPoint shifted a little bit in the direction
      // of the destination
      pt = bHit.hitPoint()
        + offset(bHit.hitPoint(), bHit.index(), edgeVec);
    }
  } while (bHit.hit());
  hits.shrink();
  return hits;
}


bool mousse::meshSearch::isInside(const point& p) const
{
  return (boundaryTree().getVolumeType(p) == volumeType::INSIDE);
}


// Delete all storage
void mousse::meshSearch::clearOut()
{
  boundaryTreePtr_.clear();
  cellTreePtr_.clear();
  overallBbPtr_.clear();
}


void mousse::meshSearch::correct()
{
  clearOut();
}

