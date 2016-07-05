// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "control_mesh_refinement.hpp"
#include "cell_size_and_alignment_control.hpp"
#include "ofstream.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(controlMeshRefinement, 0);

}


// Private Member Functions 
mousse::scalar mousse::controlMeshRefinement::calcFirstDerivative
(
  const mousse::point& a,
  const scalar& cellSizeA,
  const mousse::point& b,
  const scalar& cellSizeB
) const
{
  return (cellSizeA - cellSizeB)/mag(a - b);
}


bool mousse::controlMeshRefinement::detectEdge
(
  const mousse::point& startPt,
  const mousse::point& endPt,
  pointHit& pointFound,
  const scalar tolSqr,
  const scalar secondDerivTolSqr
) const
{
  mousse::point a{startPt};
  mousse::point b{endPt};
  mousse::point midPoint = (a + b)/2.0;
  label nIterations = 0;
  while (true) {
    nIterations++;
    if (magSqr(a - b) < tolSqr) {
      pointFound.setPoint(midPoint);
      pointFound.setHit();
      return true;
    }
    // Split into two regions
    scalar cellSizeA = sizeControls_.cellSize(a);
    scalar cellSizeB = sizeControls_.cellSize(b);
    scalar cellSizeMid = sizeControls_.cellSize(midPoint);
    // Region 1
    mousse::point midPoint1 = (a + midPoint)/2.0;
    const scalar cellSizeMid1 = sizeControls_.cellSize(midPoint1);
    scalar secondDerivative1 =
      calcSecondDerivative
      (
        a,
        cellSizeA,
        midPoint1,
        cellSizeMid1,
        midPoint,
        cellSizeMid
      );
    // Region 2
    mousse::point midPoint2 = (midPoint + b)/2.0;
    const scalar cellSizeMid2 = sizeControls_.cellSize(midPoint2);
    scalar secondDerivative2 =
      calcSecondDerivative
      (
        midPoint,
        cellSizeMid,
        midPoint2,
        cellSizeMid2,
        b,
        cellSizeB
      );
    // Neither region appears to have an inflection
    // To be sure should use higher order derivatives
    if (magSqr(secondDerivative1) < secondDerivTolSqr
        && magSqr(secondDerivative2) < secondDerivTolSqr) {
      return false;
    }
    // Pick region with greatest second derivative
    if (magSqr(secondDerivative1) > magSqr(secondDerivative2)) {
      b = midPoint;
      midPoint = midPoint1;
    } else {
      a = midPoint;
      midPoint = midPoint2;
    }
  }
}


mousse::pointHit mousse::controlMeshRefinement::findDiscontinuities
(
  const linePointRef& l
) const
{
  pointHit p{point::max};
  const scalar tolSqr{sqr(1e-3)};
  const scalar secondDerivTolSqr{sqr(1e-3)};
  detectEdge
    (
      l.start(),
      l.end(),
      p,
      tolSqr,
      secondDerivTolSqr
    );
  return p;
}


// Protected Member Functions 

// Constructors 
mousse::controlMeshRefinement::controlMeshRefinement
(
  cellShapeControl& shapeController
)
:
  shapeController_{shapeController},
  mesh_{shapeController.shapeControlMesh()},
  sizeControls_{shapeController.sizeAndAlignment()},
  geometryToConformTo_{sizeControls_.geometryToConformTo()}
{}


// Destructor 
mousse::controlMeshRefinement::~controlMeshRefinement()
{}


// Member Functions 
void mousse::controlMeshRefinement::initialMeshPopulation
(
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
{
  if (shapeController_.shapeControlMesh().vertexCount() > 0) {
    // Mesh already populated.
    Info << "Cell size and alignment mesh already populated." << endl;
    return;
  }
  autoPtr<boundBox> overallBoundBox;
  Map<label> priorityMap;
  const PtrList<cellSizeAndAlignmentControl>& controlFunctions =
    sizeControls_.controlFunctions();
  FOR_ALL(controlFunctions, fI) {
    const cellSizeAndAlignmentControl& controlFunction =
      controlFunctions[fI];
    const Switch& forceInsertion =
      controlFunction.forceInitialPointInsertion();
    Info << "Inserting points from " << controlFunction.name()
      << " (" << controlFunction.type() << ")" << endl;
    Info << "    Force insertion is " << forceInsertion.asText() << endl;
    pointField pts;
    scalarField sizes;
    triadField alignments;
    controlFunction.initialVertices(pts, sizes, alignments);
    Info << "    Got initial vertices list of size " << pts.size() << endl;
    List<Vb> vertices{pts.size()};
    // Clip the minimum size
    for (label vI = 0; vI < pts.size(); ++vI) {
      vertices[vI] = Vb{pts[vI], Vb::vtInternalNearBoundary};
      label maxPriority = -1;
      scalar size = sizeControls_.cellSize(pts[vI], maxPriority);
      if (maxPriority > controlFunction.maxPriority()) {
        vertices[vI].targetCellSize() =
          max(size, shapeController_.minimumCellSize());
      } else {
        vertices[vI].targetCellSize() =
          max(sizes[vI], shapeController_.minimumCellSize());
      }
      vertices[vI].alignment() = alignments[vI];
    }
    Info << "    Clipped minimum size" << endl;
    pts.clear();
    sizes.clear();
    alignments.clear();
    PackedBoolList keepVertex{vertices.size(), true};
    FOR_ALL(vertices, vI) {
      bool keep = true;
      pointFromPoint pt = topoint(vertices[vI].point());
      if (Pstream::parRun()) {
        keep = decomposition().positionOnThisProcessor(pt);
      }
      if (keep && geometryToConformTo_.wellOutside(pt, SMALL)) {
        keep = false;
      }
      if (!keep) {
        keepVertex[vI] = false;
      }
    }
    inplaceSubset(keepVertex, vertices);
    const label preInsertedSize = mesh_.number_of_vertices();
    Info << "    Check sizes" << endl;
    FOR_ALL(vertices, vI) {
      bool insertPoint = false;
      pointFromPoint pt(topoint(vertices[vI].point()));
      if (mesh_.dimension() < 3
          || mesh_.is_infinite(mesh_.locate(vertices[vI].point()))) {
        insertPoint = true;
      }
      const scalar interpolatedCellSize = shapeController_.cellSize(pt);
      const triad interpolatedAlignment =
        shapeController_.cellAlignment(pt);
      const scalar calculatedCellSize = vertices[vI].targetCellSize();
      const triad calculatedAlignment = vertices[vI].alignment();
      if (debug) {
        Info << "Point = " << pt << nl
          << "  Size(interp) = " << interpolatedCellSize << nl
          << "    Size(calc) = " << calculatedCellSize << nl
          << " Align(interp) = " << interpolatedAlignment << nl
          << "   Align(calc) = " << calculatedAlignment << nl
          << endl;
      }
      const scalar sizeDiff =
        mag(interpolatedCellSize - calculatedCellSize);
      const scalar alignmentDiff =
        diff(interpolatedAlignment, calculatedAlignment);
      if (debug) {
        Info << "    size difference = " << sizeDiff << nl
          << ", alignment difference = " << alignmentDiff << endl;
      }
      // @todo Also need to base it on the alignments
      if (sizeDiff/interpolatedCellSize > 0.1 || alignmentDiff > 0.15) {
        insertPoint = true;
      }
      if (forceInsertion || insertPoint) {
        const label oldSize = mesh_.vertexCount();
        cellShapeControlMesh::Vertex_handle insertedVert =
          mesh_.insert
          (
            pt,
            calculatedCellSize,
            vertices[vI].alignment(),
            Vb::vtInternalNearBoundary
          );
        if (oldSize == mesh_.vertexCount() - 1) {
          priorityMap.insert
            (
              insertedVert->index(),
              controlFunction.maxPriority()
            );
        }
      }
    }
    //mesh_.rangeInsertWithInfo(vertices.begin(), vertices.end());
    Info << "    Inserted "
      << returnReduce
         (
           label(mesh_.number_of_vertices()) - preInsertedSize,
           sumOp<label>()
         )
      << "/" << returnReduce(vertices.size(), sumOp<label>())
      << endl;
  }
  FOR_ALL(controlFunctions, fI) {
    const cellSizeAndAlignmentControl& controlFunction =
      controlFunctions[fI];
    const Switch& forceInsertion =
      controlFunction.forceInitialPointInsertion();
    Info << "Inserting points from " << controlFunction.name()
      << " (" << controlFunction.type() << ")" << endl;
    Info << "    Force insertion is " << forceInsertion.asText() << endl;
    DynamicList<mousse::point> extraPts;
    DynamicList<scalar> extraSizes;
    controlFunction.cellSizeFunctionVertices(extraPts, extraSizes);
    List<Vb> vertices{extraPts.size()};
    // Clip the minimum size
    for (label vI = 0; vI < extraPts.size(); ++vI) {
      vertices[vI] = Vb{extraPts[vI], Vb::vtUnassigned};
      label maxPriority = -1;
      scalar size = sizeControls_.cellSize(extraPts[vI], maxPriority);
      if (maxPriority > controlFunction.maxPriority()) {
        vertices[vI].targetCellSize() =
          max(size, shapeController_.minimumCellSize());
      } else if (maxPriority == controlFunction.maxPriority()) {
        vertices[vI].targetCellSize() =
          max(min(extraSizes[vI], size), shapeController_.minimumCellSize());
      } else {
        vertices[vI].targetCellSize() =
          max(extraSizes[vI], shapeController_.minimumCellSize());
      }
    }
    PackedBoolList keepVertex{vertices.size(), true};
    FOR_ALL(vertices, vI) {
      bool keep = true;
      pointFromPoint pt = topoint(vertices[vI].point());
      if (Pstream::parRun()) {
        keep = decomposition().positionOnThisProcessor(pt);
      }
      if (keep && geometryToConformTo_.wellOutside(pt, SMALL)) {
        keep = false;
      }
      if (!keep) {
        keepVertex[vI] = false;
      }
    }
    inplaceSubset(keepVertex, vertices);
    const label preInsertedSize = mesh_.number_of_vertices();
    FOR_ALL(vertices, vI) {
      bool insertPoint = false;
      pointFromPoint pt{topoint(vertices[vI].point())};
      if (mesh_.dimension() < 3
          || mesh_.is_infinite(mesh_.locate(vertices[vI].point()))) {
        insertPoint = true;
      }
      const scalar interpolatedCellSize = shapeController_.cellSize(pt);
      const scalar calculatedCellSize = vertices[vI].targetCellSize();
      if (debug) {
        Info << "Point = " << pt << nl
          << "  Size(interp) = " << interpolatedCellSize << nl
          << "    Size(calc) = " << calculatedCellSize << nl
          << endl;
      }
      const scalar sizeDiff = mag(interpolatedCellSize - calculatedCellSize);
      if (debug) {
        Info << "    size difference = " << sizeDiff << endl;
      }
      // @todo Also need to base it on the alignments
      if (sizeDiff/interpolatedCellSize > 0.1) {
        insertPoint = true;
      }
      if (forceInsertion || insertPoint) {
        mesh_.insert
          (
            pt,
            calculatedCellSize,
            vertices[vI].alignment(),
            Vb::vtInternal
          );
      }
    }
    Info << "    Inserted extra points "
      << returnReduce
         (
           label(mesh_.number_of_vertices()) - preInsertedSize,
           sumOp<label>()
         )
      << "/" << returnReduce(vertices.size(), sumOp<label>())
      << endl;
  }
}


mousse::label mousse::controlMeshRefinement::refineMesh
(
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
{
  Info << "Iterate over "
    << returnReduce(label(mesh_.number_of_finite_edges()), sumOp<label>())
    << " cell size mesh edges" << endl;
  DynamicList<Vb> verts{static_cast<label>(mesh_.number_of_vertices())};
  label count = 0;
  for (auto eit = mesh_.finite_edges_begin();
       eit != mesh_.finite_edges_end();
       ++eit) {
    if (count % 10000 == 0) {
      Info << count << " edges, inserted " << verts.size()
        << " Time = " << mesh_.time().elapsedCpuTime()
        << endl;
    }
    count++;
    CellSizeDelaunay::Cell_handle c = eit->first;
    CellSizeDelaunay::Vertex_handle vA = c->vertex(eit->second);
    CellSizeDelaunay::Vertex_handle vB = c->vertex(eit->third);
    if (mesh_.is_infinite(vA)
        || mesh_.is_infinite(vB)
        || (vA->referred() && vB->referred())
        || (vA->referred() && (vA->procIndex() > vB->procIndex()))
        || (vB->referred() && (vB->procIndex() > vA->procIndex()))) {
      continue;
    }
    pointFromPoint ptA{topoint(vA->point())};
    pointFromPoint ptB{topoint(vB->point())};
    linePointRef l{ptA, ptB};
    const pointHit hitPt = findDiscontinuities(l);
    if (hitPt.hit()) {
      const mousse::point& pt = hitPt.hitPoint();
      if (!geometryToConformTo_.inside(pt)) {
        continue;
      }
      if (Pstream::parRun()) {
        if (!decomposition().positionOnThisProcessor(pt)) {
          continue;
        }
      }
      verts.append
      (
        Vb
        {
          toPoint(pt),
          Vb::vtInternal
        }
      );
      verts.last().targetCellSize() = sizeControls_.cellSize(pt);
      verts.last().alignment() = triad::unset;
    }
  }
  mesh_.insertPoints(verts, false);
  return verts.size();
}

