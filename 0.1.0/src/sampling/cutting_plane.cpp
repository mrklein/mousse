// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cutting_plane.hpp"
#include "primitive_mesh.hpp"
#include "line_point_ref.hpp"
#include "mesh_tools.hpp"


// Static Data Members
// Set values for what is close to zero and what is considered to
// be positive (and not just rounding noise)
//! \cond localScope
const mousse::scalar zeroish  = mousse::SMALL;
const mousse::scalar positive = mousse::SMALL * 1E3;
//! \endcond


// Private Member Functions 
void mousse::cuttingPlane::calcCutCells
(
  const primitiveMesh& mesh,
  const scalarField& dotProducts,
  const labelUList& cellIdLabels
)
{
  const labelListList& cellEdges = mesh.cellEdges();
  const edgeList& edges = mesh.edges();
  label listSize = cellEdges.size();
  if (notNull(cellIdLabels)) {
    listSize = cellIdLabels.size();
  }
  cutCells_.setSize(listSize);
  label cutcellI{0};
  // Find the cut cells by detecting any cell that uses points with
  // opposing dotProducts.
  for (label listI = 0; listI < listSize; ++listI) {
    label cellI = listI;
    if (notNull(cellIdLabels)) {
      cellI = cellIdLabels[listI];
    }
    const labelList& cEdges = cellEdges[cellI];
    label nCutEdges = 0;
    FOR_ALL(cEdges, i) {
      const edge& e = edges[cEdges[i]];
      if ((dotProducts[e[0]] < zeroish && dotProducts[e[1]] > positive)
          || (dotProducts[e[1]] < zeroish && dotProducts[e[0]] > positive)) {
        nCutEdges++;
        if (nCutEdges > 2) {
          cutCells_[cutcellI++] = cellI;
          break;
        }
      }
    }
  }
  // Set correct list size
  cutCells_.setSize(cutcellI);
}


void mousse::cuttingPlane::intersectEdges
(
  const primitiveMesh& mesh,
  const scalarField& dotProducts,
  List<label>& edgePoint
)
{
  // Use the dotProducts to find out the cut edges.
  const edgeList& edges = mesh.edges();
  const pointField& points = mesh.points();
  // Per edge -1 or the label of the intersection point
  edgePoint.setSize(edges.size());
  DynamicList<point> dynCuttingPoints{4*cutCells_.size()};
  FOR_ALL(edges, edgeI) {
    const edge& e = edges[edgeI];
    if ((dotProducts[e[0]] < zeroish && dotProducts[e[1]] > positive)
        || (dotProducts[e[1]] < zeroish && dotProducts[e[0]] > positive)) {
      // Edge is cut
      edgePoint[edgeI] = dynCuttingPoints.size();
      const point& p0 = points[e[0]];
      const point& p1 = points[e[1]];
      scalar alpha = lineIntersect(linePointRef(p0, p1));
      if (alpha < zeroish) {
        dynCuttingPoints.append(p0);
      } else if (alpha >= 1.0) {
        dynCuttingPoints.append(p1);
      } else {
        dynCuttingPoints.append((1-alpha)*p0 + alpha*p1);
      }
    } else {
      edgePoint[edgeI] = -1;
    }
  }
  this->storedPoints().transfer(dynCuttingPoints);
}


bool mousse::cuttingPlane::walkCell
(
  const primitiveMesh& mesh,
  const labelUList& edgePoint,
  const label cellI,
  const label startEdgeI,
  DynamicList<label>& faceVerts
)
{
  label faceI = -1;
  label edgeI = startEdgeI;
  label nIter = 0;
  faceVerts.clear();
  do {
    faceVerts.append(edgePoint[edgeI]);
    // Cross edge to other face
    faceI = meshTools::otherFace(mesh, cellI, faceI, edgeI);
    // Find next cut edge on face.
    const labelList& fEdges = mesh.faceEdges()[faceI];
    label nextEdgeI = -1;
    //Note: here is where we should check for whether there are more
    // than 2 intersections with the face (warped/non-convex face).
    // If so should e.g. decompose the cells on both faces and redo
    // the calculation.
    FOR_ALL(fEdges, i) {
      label edge2I = fEdges[i];
      if (edge2I != edgeI && edgePoint[edge2I] != -1) {
        nextEdgeI = edge2I;
        break;
      }
    }
    if (nextEdgeI == -1) {
      // Did not find another cut edge on faceI. Do what?
      WARNING_IN("mousse::cuttingPlane::walkCell")
        << "Did not find closed walk along surface of cell " << cellI
        << " starting from edge " << startEdgeI
        << " in " << nIter << " iterations." << nl
        << "Collected cutPoints so far:" << faceVerts
        << endl;
      return false;
    }
    edgeI = nextEdgeI;
    nIter++;
    if (nIter > 1000) {
      WARNING_IN("mousse::cuttingPlane::walkCell")
        << "Did not find closed walk along surface of cell " << cellI
        << " starting from edge " << startEdgeI
        << " in " << nIter << " iterations." << nl
        << "Collected cutPoints so far:" << faceVerts
        << endl;
      return false;
    }
  } while (edgeI != startEdgeI);
  if (faceVerts.size() >= 3) {
    return true;
  } else {
    WARNING_IN("mousse::cuttingPlane::walkCell")
      << "Did not find closed walk along surface of cell " << cellI
      << " starting from edge " << startEdgeI << nl
      << "Collected cutPoints so far:" << faceVerts
      << endl;
    return false;
  }
}


void mousse::cuttingPlane::walkCellCuts
(
  const primitiveMesh& mesh,
  const bool triangulate,
  const labelUList& edgePoint
)
{
  const pointField& cutPoints = this->points();
  // use dynamic lists to handle triangulation and/or missed cuts
  DynamicList<face> dynCutFaces{cutCells_.size()};
  DynamicList<label> dynCutCells{cutCells_.size()};
  // scratch space for calculating the face vertices
  DynamicList<label> faceVerts{10};
  FOR_ALL(cutCells_, i) {
    label cellI = cutCells_[i];
    // Find the starting edge to walk from.
    const labelList& cEdges = mesh.cellEdges()[cellI];
    label startEdgeI = -1;
    FOR_ALL(cEdges, cEdgeI) {
      label edgeI = cEdges[cEdgeI];
      if (edgePoint[edgeI] != -1) {
        startEdgeI = edgeI;
        break;
      }
    }
    // Check for the unexpected ...
    if (startEdgeI == -1) {
      FATAL_ERROR_IN("mousse::cuttingPlane::walkCellCuts(..)")
        << "Cannot find cut edge for cut cell " << cellI
        << abort(FatalError);
    }
    // Walk from starting edge around the circumference of the cell.
    bool okCut = walkCell
    (
      mesh,
      edgePoint,
      cellI,
      startEdgeI,
      faceVerts
    );
    if (okCut) {
      face f{faceVerts};
      // Orient face to point in the same direction as the plane normal
      if ((f.normal(cutPoints) & normal()) < 0) {
        f.flip();
      }
      // the cut faces are usually quite ugly, so optionally triangulate
      if (triangulate) {
        label nTri = f.triangles(cutPoints, dynCutFaces);
        while (nTri--) {
          dynCutCells.append(cellI);
        }
      } else {
        dynCutFaces.append(f);
        dynCutCells.append(cellI);
      }
    }
  }
  this->storedFaces().transfer(dynCutFaces);
  cutCells_.transfer(dynCutCells);
}


// Constructors 

// Construct without cutting
mousse::cuttingPlane::cuttingPlane(const plane& pln)
:
  plane{pln}
{}


// Construct from plane and mesh reference, restricted to a list of cells
mousse::cuttingPlane::cuttingPlane
(
  const plane& pln,
  const primitiveMesh& mesh,
  const bool triangulate,
  const labelUList& cellIdLabels
)
:
  plane{pln}
{
  reCut(mesh, triangulate, cellIdLabels);
}


// Member Functions 
void mousse::cuttingPlane::reCut
(
  const primitiveMesh& mesh,
  const bool triangulate,
  const labelUList& cellIdLabels
)
{
  MeshStorage::clear();
  cutCells_.clear();
  const scalarField dotProducts{(mesh.points() - refPoint()) & normal()};
  // Determine cells that are (probably) cut.
  calcCutCells(mesh, dotProducts, cellIdLabels);
  // Determine cutPoints and return list of edge cuts.
  // per edge -1 or the label of the intersection point
  labelList edgePoint;
  intersectEdges(mesh, dotProducts, edgePoint);
  // Do topological walk around cell to find closed loop.
  walkCellCuts(mesh, triangulate, edgePoint);
}


void mousse::cuttingPlane::remapFaces
(
  const labelUList& faceMap
)
{
  // recalculate the cells cut
  if (notNull(faceMap) && faceMap.size()) {
    MeshStorage::remapFaces(faceMap);
    List<label> newCutCells{faceMap.size()};
    FOR_ALL(faceMap, faceI) {
      newCutCells[faceI] = cutCells_[faceMap[faceI]];
    }
    cutCells_.transfer(newCutCells);
  }
}


// Member Operators 
void mousse::cuttingPlane::operator=(const cuttingPlane& rhs)
{
  // Check for assignment to self
  if (this == &rhs) {
    FATAL_ERROR_IN ("mousse::cuttingPlane::operator=(const cuttingPlane&)")
      << "Attempted assignment to self"
      << abort(FatalError);
  }
  static_cast<MeshStorage&>(*this) = rhs;
  static_cast<plane&>(*this) = rhs;
  cutCells_ = rhs.cutCells();
}

