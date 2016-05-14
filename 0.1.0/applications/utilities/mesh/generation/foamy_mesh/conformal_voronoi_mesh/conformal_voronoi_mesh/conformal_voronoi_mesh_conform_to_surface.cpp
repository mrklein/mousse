// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "conformal_voronoi_mesh.hpp"
#include "background_mesh_decomposition.hpp"
#include "vector_tools.hpp"
#include "indexed_cell_checks.hpp"
#include "iomanip.hpp"
#include "obj_stream.hpp"


using namespace mousse::vectorTools;


const mousse::scalar mousse::conformalVoronoiMesh::searchConeAngle
  = mousse::cos(degToRad(30));

const mousse::scalar mousse::conformalVoronoiMesh::searchAngleOppositeSurface
  = mousse::cos(degToRad(150));


// Private Member Functions 
void mousse::conformalVoronoiMesh::conformToSurface()
{
  this->resetCellCount();
  // Index the cells
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit) {
    cit->cellIndex() = Cb::ctUnassigned;
  }
  if (!reconformToSurface()) {
    // Reinsert stored surface conformation
    reinsertSurfaceConformation();
    if (Pstream::parRun()) {
      sync(decomposition().procBounds());
    }
  } else {
    ptPairs_.clear();
    // Rebuild, insert and store new surface conformation
    buildSurfaceConformation();
    if (distributeBackground(*this)) {
      if (Pstream::parRun()) {
        sync(decomposition().procBounds());
      }
    }
    // Do not store the surface conformation until after it has been
    // (potentially) redistributed.
    storeSurfaceConformation();
  }
}


bool mousse::conformalVoronoiMesh::reconformToSurface() const
{
  const auto& freq =
    foamyHexMeshControls().surfaceConformationRebuildFrequency();
  return runTime_.timeIndex() % freq == 0;
}


// @todo Investigate topological tests
mousse::label mousse::conformalVoronoiMesh::findVerticesNearBoundaries()
{
  label countNearBoundaryVertices = 0;
  for (auto fit = finite_facets_begin();
       fit != finite_facets_end();
       ++fit) {
    Cell_handle c1 = fit->first;
    Cell_handle c2 = fit->first->neighbor(fit->second);
    if (is_infinite(c1) || is_infinite(c2)) {
      continue;
    }
    pointFromPoint dE0 = c1->dual();
    pointFromPoint dE1 = c2->dual();
    if (!geometryToConformTo_.findSurfaceAnyIntersection(dE0, dE1)) {
      continue;
    }
    for (label cellI = 0; cellI < 4; ++cellI) {
      Vertex_handle v = c1->vertex(cellI);
      if (!is_infinite(v) && v->internalPoint() && fit->second != cellI) {
        v->setNearBoundary();
      }
    }
    for (label cellI = 0; cellI < 4; ++cellI) {
      Vertex_handle v = c2->vertex(cellI);
      if (!is_infinite(v) && v->internalPoint() && fit->second != cellI) {
        v->setNearBoundary();
      }
    }
  }
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit) {
    if (vit->nearBoundary()) {
      countNearBoundaryVertices++;
    }
  }
  return countNearBoundaryVertices;
}


void mousse::conformalVoronoiMesh::buildSurfaceConformation()
{
  timeCheck("Start buildSurfaceConformation");
  Info << nl
    << "Rebuilding surface conformation for more iterations"
    << endl;
  existingEdgeLocations_.clearStorage();
  existingSurfacePtLocations_.clearStorage();
  buildEdgeLocationTree(existingEdgeLocations_);
  buildSurfacePtLocationTree(existingSurfacePtLocations_);
  label initialTotalHits = 0;
  // Surface protrusion conformation is done in two steps.
  // 1. the dual edges (of all internal vertices) can stretch to
  //    'infinity' so any intersection would be badly behaved. So
  //    just find the nearest point on the geometry and insert point
  //    pairs.
  // Now most of the surface conformation will be done with some
  // residual protrusions / incursions.
  // 2. find any segments of dual edges outside the geometry. Shoot
  //    ray from Delaunay vertex to middle of this segment and introduce
  //    point pairs. This will handle e.g.
  // protruding section of face:
  //
  //     internal
  // \             /
  // -+-----------+-- boundary
  //   \         /
  //     --------
  //
  // Shoot ray and find intersection with outside segment (x) and
  // introduce point pair (..)
  //
  //        |
  // \      .      /
  // -+-----|-----+-- boundary
  //   \    .    /
  //     ---x----
  // Find vertices near boundaries to speed up subsequent checks.
  label countNearBoundaryVertices = findVerticesNearBoundaries();
  Info << "    Vertices marked as being near a boundary: "
    << returnReduce(countNearBoundaryVertices, sumOp<label>())
    << " (estimated)" << endl;
  timeCheck("After set near boundary");
  const scalar edgeSearchDistCoeffSqr =
    foamyHexMeshControls().edgeSearchDistCoeffSqr();
  const scalar surfacePtReplaceDistCoeffSqr =
    foamyHexMeshControls().surfacePtReplaceDistCoeffSqr();
  const label AtoV =
    static_cast<label>(6/mousse::pow(scalar(number_of_vertices()), 3));

  // Initial surface protrusion conformation - nearest surface point
  {
    pointIndexHitAndFeatureDynList featureEdgeHits{AtoV/4};
    pointIndexHitAndFeatureDynList surfaceHits{AtoV};
    DynamicList<label> edgeToTreeShape{AtoV/4};
    DynamicList<label> surfaceToTreeShape{AtoV};
    Map<scalar> surfacePtToEdgePtDist{AtoV/4};
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         vit++) {
      if (vit->nearBoundary()) {
        pointIndexHitAndFeatureDynList surfaceIntersections{AtoV};
        if (dualCellSurfaceAllIntersections(vit, surfaceIntersections)) {
          addSurfaceAndEdgeHits
            (
              topoint(vit->point()),
              surfaceIntersections,
              surfacePtReplaceDistCoeffSqr,
              edgeSearchDistCoeffSqr,
              surfaceHits,
              featureEdgeHits,
              surfaceToTreeShape,
              edgeToTreeShape,
              surfacePtToEdgePtDist,
              true
            );
        } else {
          vit->setInternal();
          countNearBoundaryVertices--;
        }
      }
    }
    Info << "    Vertices marked as being near a boundary: "
      << returnReduce(countNearBoundaryVertices, sumOp<label>())
      << " (after dual surface intersection)" << endl;
    label nVerts = number_of_vertices();
    label nSurfHits = surfaceHits.size();
    label nFeatEdHits = featureEdgeHits.size();
    if (Pstream::parRun()) {
      reduce(nVerts, sumOp<label>());
      reduce(nSurfHits, sumOp<label>());
      reduce(nFeatEdHits, sumOp<label>());
    }
    Info << nl << "Initial conformation" << nl
      << "    Number of vertices " << nVerts << nl
      << "    Number of surface hits " << nSurfHits << nl
      << "    Number of edge hits " << nFeatEdHits
      << endl;
    // In parallel, synchronise the surface trees
    if (Pstream::parRun()) {
      synchroniseSurfaceTrees(surfaceToTreeShape, surfaceHits);
    }
    DynamicList<Vb> pts{2*surfaceHits.size() + 3*featureEdgeHits.size()};
    insertSurfacePointPairs
      (
        surfaceHits,
        "surfaceConformationLocations_initial.obj",
        pts
      );
    // In parallel, synchronise the edge trees
    if (Pstream::parRun()) {
      synchroniseEdgeTrees(edgeToTreeShape, featureEdgeHits);
    }
    insertEdgePointGroups
      (
        featureEdgeHits,
        "edgeConformationLocations_initial.obj",
        pts
      );
    pts.shrink();
    Map<label> oldToNewIndices = insertPointPairs(pts, true, true);
    // Re-index the point pairs
    ptPairs_.reIndex(oldToNewIndices);
    //writePointPairs("pointPairs_initial.obj");
    // Remove location from surface/edge tree
    timeCheck("After initial conformation");
    initialTotalHits = nSurfHits + nFeatEdHits;
  }
  // Remember which vertices were referred to each processor so only updates
  // are sent.
  PtrList<labelPairHashSet> referralVertices{Pstream::nProcs()};
  // Store the vertices that have been received and added from each processor
  // already so that there is no attempt to add them more than once.
  autoPtr<labelPairHashSet> receivedVertices;
  if (Pstream::parRun()) {
    FOR_ALL(referralVertices, procI) {
      if (procI != Pstream::myProcNo()) {
        referralVertices.set
        (
          procI,
          new labelPairHashSet
          {
            static_cast<label>(number_of_vertices())/Pstream::nProcs()
          }
        );
      }
    }
    receivedVertices.set
    (
      new labelPairHashSet
      {
        static_cast<label>(number_of_vertices())/Pstream::nProcs()
      }
    );
    // Build the parallel interface the initial surface conformation
    sync
      (
        decomposition_().procBounds(),
        referralVertices,
        receivedVertices()
      );
  }
  label iterationNo = 0;
  label maxIterations = foamyHexMeshControls().maxConformationIterations();
  scalar iterationToInitialHitRatioLimit =
    foamyHexMeshControls().iterationToInitialHitRatioLimit();
  label hitLimit =
    static_cast<label>(iterationToInitialHitRatioLimit*initialTotalHits);
  Info << nl << "Stopping iterations when: " << nl
    << "    total number of hits drops below "
    << iterationToInitialHitRatioLimit
    << " of initial hits (" << hitLimit << ")" << nl
    << " or " << nl
    << "    maximum number of iterations (" << maxIterations
    << ") is reached"
    << endl;
  // Set totalHits to a large enough positive value to enter the while loop on
  // the first iteration
  label totalHits = initialTotalHits;
  while (totalHits > 0 && totalHits >= hitLimit && iterationNo < maxIterations) {
    pointIndexHitAndFeatureDynList surfaceHits{AtoV/2};
    pointIndexHitAndFeatureDynList featureEdgeHits{AtoV/4};
    DynamicList<label> surfaceToTreeShape{AtoV/2};
    DynamicList<label> edgeToTreeShape{AtoV/4};
    Map<scalar> surfacePtToEdgePtDist;
    for (auto vit = finite_vertices_begin();
         vit != finite_vertices_end();
         ++vit) {
      // The initial surface conformation has already identified the
      // nearBoundary set of vertices.  Previously inserted boundary
      // points and referred internal vertices from other processors can
      // also generate protrusions and must be assessed too.
      if (vit->nearBoundary() || vit->internalBoundaryPoint()
          || (vit->internalOrBoundaryPoint() && vit->referred())) {
        pointIndexHitAndFeatureDynList surfaceIntersections{AtoV/2};
        pointIndexHit surfHit;
        label hitSurface;
        // Find segments of dual face outside the geometry and find the
        // the middle of this
        dualCellLargestSurfaceProtrusion(vit, surfHit, hitSurface);
        if (surfHit.hit()) {
          surfaceIntersections.append
          (
            pointIndexHitAndFeature(surfHit, hitSurface)
          );
          addSurfaceAndEdgeHits
            (
              topoint(vit->point()),
              surfaceIntersections,
              surfacePtReplaceDistCoeffSqr,
              edgeSearchDistCoeffSqr,
              surfaceHits,
              featureEdgeHits,
              surfaceToTreeShape,
              edgeToTreeShape,
              surfacePtToEdgePtDist,
              false
            );
        } else {
          // No surface hit detected so if internal then don't check
          // again
          if (vit->nearBoundary()) {
            vit->setInternal();
          }
        }
      } else if (vit->externalBoundaryPoint()
                 || (vit->externalBoundaryPoint() && vit->referred())) {
        pointIndexHitAndFeatureDynList surfaceIntersections{AtoV/2};
        pointIndexHit surfHit;
        label hitSurface;
        // Detect slave (external vertices) whose dual face incurs
        // into nearby (other than originating) geometry
        dualCellLargestSurfaceIncursion(vit, surfHit, hitSurface);
        if (surfHit.hit()) {
          surfaceIntersections.append
          (
            pointIndexHitAndFeature(surfHit, hitSurface)
          );
          addSurfaceAndEdgeHits
            (
              topoint(vit->point()),
              surfaceIntersections,
              surfacePtReplaceDistCoeffSqr,
              edgeSearchDistCoeffSqr,
              surfaceHits,
              featureEdgeHits,
              surfaceToTreeShape,
              edgeToTreeShape,
              surfacePtToEdgePtDist,
              false
            );
        }
      }
    }
    label nVerts = number_of_vertices();
    label nSurfHits = surfaceHits.size();
    label nFeatEdHits = featureEdgeHits.size();
    if (Pstream::parRun()) {
      reduce(nVerts, sumOp<label>());
      reduce(nSurfHits, sumOp<label>());
      reduce(nFeatEdHits, sumOp<label>());
    }
    Info << nl << "Conformation iteration " << iterationNo << nl
      << "    Number of vertices " << nVerts << nl
      << "    Number of surface hits " << nSurfHits << nl
      << "    Number of edge hits " << nFeatEdHits
      << endl;
    totalHits = nSurfHits + nFeatEdHits;
    label nNotInserted = 0;
    if (totalHits > 0) {
      // In parallel, synchronise the surface trees
      if (Pstream::parRun()) {
        nNotInserted +=
          synchroniseSurfaceTrees(surfaceToTreeShape, surfaceHits);
      }
      DynamicList<Vb> pts
      {
        2*surfaceHits.size() + 3*featureEdgeHits.size()
      };
      insertSurfacePointPairs
        (
          surfaceHits,
          "surfaceConformationLocations_" + name(iterationNo) + ".obj",
          pts
        );
      // In parallel, synchronise the edge trees
      if (Pstream::parRun()) {
        nNotInserted +=
          synchroniseEdgeTrees(edgeToTreeShape, featureEdgeHits);
      }
      insertEdgePointGroups
        (
          featureEdgeHits,
          "edgeConformationLocations_" + name(iterationNo) + ".obj",
          pts
        );
      pts.shrink();
      Map<label> oldToNewIndices = insertPointPairs(pts, true, true);
      // Reindex the point pairs
      ptPairs_.reIndex(oldToNewIndices);
      //writePointPairs("pointPairs_" + name(iterationNo) + ".obj");
      if (Pstream::parRun()) {
        sync
          (
            decomposition_().procBounds(),
            referralVertices,
            receivedVertices()
          );
      }
    }
    timeCheck("Conformation iteration " + name(iterationNo));
    iterationNo++;
    if (iterationNo == maxIterations) {
      WARNING_IN("conformalVoronoiMesh::conformToSurface()")
        << "Maximum surface conformation iterations ("
        << maxIterations <<  ") reached." << endl;
    }
    if (totalHits <= nNotInserted) {
      Info << nl << "Total hits (" << totalHits
        << ") less than number of failed insertions (" << nNotInserted
        << "), stopping iterations" << endl;
      break;
    }
    if (totalHits < hitLimit) {
      Info << nl << "Total hits (" << totalHits
        << ") less than limit (" << hitLimit
        << "), stopping iterations" << endl;
    }
  }
  edgeLocationTreePtr_.clear();
  surfacePtLocationTreePtr_.clear();
}


mousse::label mousse::conformalVoronoiMesh::synchroniseSurfaceTrees
(
  const DynamicList<label>& surfaceToTreeShape,
  pointIndexHitAndFeatureList& surfaceHits
)
{
  Info << "    Surface tree synchronisation" << endl;
  pointIndexHitAndFeatureDynList synchronisedSurfLocations
  {
    surfaceHits.size()
  };
  List<pointIndexHitAndFeatureDynList> procSurfLocations{Pstream::nProcs()};
  procSurfLocations[Pstream::myProcNo()] = surfaceHits;
  Pstream::gatherList(procSurfLocations);
  Pstream::scatterList(procSurfLocations);
  List<labelHashSet> hits{Pstream::nProcs()};
  label nStoppedInsertion = 0;
  // Do the nearness tests here
  for (label procI = 0; procI < Pstream::nProcs(); ++procI) {
    // Skip own points
    if (procI >= Pstream::myProcNo()) {
      continue;
    }
    const pointIndexHitAndFeatureList& otherSurfEdges =
      procSurfLocations[procI];
    FOR_ALL(otherSurfEdges, peI) {
      const mousse::point& pt = otherSurfEdges[peI].first().hitPoint();
      pointIndexHit nearest;
      pointIsNearSurfaceLocation(pt, nearest);
      pointIndexHit nearestEdge;
      pointIsNearFeatureEdgeLocation(pt, nearestEdge);
      if (nearest.hit() || nearestEdge.hit()) {
        nStoppedInsertion++;
        if (!hits[procI].found(peI)) {
          hits[procI].insert(peI);
        }
      }
    }
  }
  Pstream::listCombineGather(hits, plusEqOp<labelHashSet>());
  Pstream::listCombineScatter(hits);
  FOR_ALL(surfaceHits, eI) {
    if (!hits[Pstream::myProcNo()].found(eI)) {
      synchronisedSurfLocations.append(surfaceHits[eI]);
    } else {
      surfacePtLocationTreePtr_().remove(surfaceToTreeShape[eI]);
    }
  }
  const label nNotInserted = returnReduce(nStoppedInsertion, sumOp<label>());
  Info << "        Not inserting total of " << nNotInserted << " locations"
    << endl;
  surfaceHits = synchronisedSurfLocations;
  return nNotInserted;
}


mousse::label mousse::conformalVoronoiMesh::synchroniseEdgeTrees
(
  const DynamicList<label>& edgeToTreeShape,
  pointIndexHitAndFeatureList& featureEdgeHits
)
{
  Info << "    Edge tree synchronisation" << endl;
  pointIndexHitAndFeatureDynList synchronisedEdgeLocations
  {
    featureEdgeHits.size()
  };
  List<pointIndexHitAndFeatureDynList> procEdgeLocations{Pstream::nProcs()};
  procEdgeLocations[Pstream::myProcNo()] = featureEdgeHits;
  Pstream::gatherList(procEdgeLocations);
  Pstream::scatterList(procEdgeLocations);
  List<labelHashSet> hits{Pstream::nProcs()};
  label nStoppedInsertion = 0;
  // Do the nearness tests here
  for (label procI = 0; procI < Pstream::nProcs(); ++procI) {
    // Skip own points
    if (procI >= Pstream::myProcNo()) {
      continue;
    }
    pointIndexHitAndFeatureList& otherProcEdges = procEdgeLocations[procI];
    FOR_ALL(otherProcEdges, peI) {
      const mousse::point& pt = otherProcEdges[peI].first().hitPoint();
      pointIndexHit nearest;
      pointIsNearFeatureEdgeLocation(pt, nearest);
      if (nearest.hit()) {
        nStoppedInsertion++;
        if (!hits[procI].found(peI)) {
          hits[procI].insert(peI);
        }
      }
    }
  }
  Pstream::listCombineGather(hits, plusEqOp<labelHashSet>());
  Pstream::listCombineScatter(hits);
  FOR_ALL(featureEdgeHits, eI) {
    if (!hits[Pstream::myProcNo()].found(eI)) {
      synchronisedEdgeLocations.append(featureEdgeHits[eI]);
    } else {
      edgeLocationTreePtr_().remove(edgeToTreeShape[eI]);
    }
  }
  const label nNotInserted = returnReduce(nStoppedInsertion, sumOp<label>());
  Info << "        Not inserting total of " << nNotInserted << " locations"
    << endl;
  featureEdgeHits = synchronisedEdgeLocations;
  return nNotInserted;
}


bool mousse::conformalVoronoiMesh::surfaceLocationConformsToInside
(
  const pointIndexHitAndFeature& info
) const
{
  if (info.first().hit()) {
    vectorField norm{1};
    geometryToConformTo_.getNormal
      (
        info.second(),
        List<pointIndexHit>(1, info.first()),
        norm
      );
    const vector& n = norm[0];
    const scalar ppDist = pointPairDistance(info.first().hitPoint());
    const mousse::point innerPoint = info.first().hitPoint() - ppDist*n;
    if (!geometryToConformTo_.inside(innerPoint)) {
      return false;
    }
    return true;
  }
  return false;
}


bool mousse::conformalVoronoiMesh::dualCellSurfaceAnyIntersection
(
  const Delaunay::Finite_vertices_iterator& vit
) const
{
  std::list<Facet> facets;
  incident_facets(vit, std::back_inserter(facets));
  for (auto fit = facets.begin();
       fit != facets.end();
       ++fit) {
    if (is_infinite(fit->first)
        || is_infinite(fit->first->neighbor(fit->second))
        || !fit->first->hasInternalPoint()
        || !fit->first->neighbor(fit->second)->hasInternalPoint()) {
      continue;
    }
    mousse::point dE0 = fit->first->dual();
    mousse::point dE1 = fit->first->neighbor(fit->second)->dual();
    if (Pstream::parRun()) {
      mousse::point& a = dE0;
      mousse::point& b = dE1;
      bool inProc = clipLineToProc(topoint(vit->point()), a, b);
      // Check for the edge passing through a surface
      if (inProc
          && geometryToConformTo_.findSurfaceAnyIntersection(a, b)) {
        return true;
      }
    } else {
      if (geometryToConformTo_.findSurfaceAnyIntersection(dE0, dE1)) {
        return true;
      }
    }
  }
  return false;
}


bool mousse::conformalVoronoiMesh::dualCellSurfaceAllIntersections
(
  const Delaunay::Finite_vertices_iterator& vit,
  pointIndexHitAndFeatureDynList& infoList
) const
{
  bool flagIntersection = false;
  std::list<Facet> facets;
  incident_facets(vit, std::back_inserter(facets));
  for (auto fit = facets.begin();
       fit != facets.end();
       ++fit) {
    if (is_infinite(fit->first)
        || is_infinite(fit->first->neighbor(fit->second))
        || !fit->first->hasInternalPoint()
        || !fit->first->neighbor(fit->second)->hasInternalPoint()) {
      continue;
    }
    // Construct the dual edge and search for intersections of the edge
    // with the surface
    mousse::point dE0 = fit->first->dual();
    mousse::point dE1 = fit->first->neighbor(fit->second)->dual();
    pointIndexHit infoIntersection;
    label hitSurfaceIntersection = -1;
    if (Pstream::parRun()) {
      bool inProc = clipLineToProc(topoint(vit->point()), dE0, dE1);
      if (!inProc) {
        continue;
      }
    }
    geometryToConformTo_.findSurfaceNearestIntersection
      (
        dE0,
        dE1,
        infoIntersection,
        hitSurfaceIntersection
      );
    if (infoIntersection.hit()) {
      vectorField norm{1};
      geometryToConformTo_.getNormal
        (
          hitSurfaceIntersection,
          List<pointIndexHit>(1, infoIntersection),
          norm
        );
      const vector& n = norm[0];
      pointFromPoint vertex = topoint(vit->point());
      const plane p{infoIntersection.hitPoint(), n};
      const plane::ray r{vertex, n};
      const scalar d = p.normalIntersect(r);
      mousse::point newPoint = vertex + d*n;
      pointIndexHitAndFeature info;
      geometryToConformTo_.findSurfaceNearest
        (
          newPoint,
          4.0*magSqr(newPoint - vertex),
          info.first(),
          info.second()
        );
      bool rejectPoint = false;
      if (!surfaceLocationConformsToInside(info)) {
        rejectPoint = true;
      }
      if (!rejectPoint && info.first().hit()) {
        if (!infoList.empty()) {
          FOR_ALL(infoList, hitI) {
            // Reject point if the point is already added
            if (infoList[hitI].first().index() == info.first().index()) {
              rejectPoint = true;
              break;
            }
            const mousse::point& p =
              infoList[hitI].first().hitPoint();
            const scalar separationDistance =
              mag(p - info.first().hitPoint());
            const scalar minSepDist =
              sqr
              (
                foamyHexMeshControls().removalDistCoeff()*targetCellSize(p)
              );
            // Reject the point if it is too close to another
            // surface point.
            // Could merge the points?
            if (separationDistance < minSepDist) {
              rejectPoint = true;
              break;
            }
          }
        }
      }
      // The normal ray from the vertex will not always result in a hit
      // because another surface may be in the way.
      if (!rejectPoint && info.first().hit()) {
        flagIntersection = true;
        infoList.append(info);
      }
    }
  }
  return flagIntersection;
}


bool mousse::conformalVoronoiMesh::clipLineToProc
(
  const mousse::point& pt,
  mousse::point& a,
  mousse::point& b
) const
{
  bool inProc = false;
  pointIndexHit findAnyIntersection = decomposition_().findLine(a, b);
  if (!findAnyIntersection.hit()) {
    pointIndexHit info = decomposition_().findLine(a, pt);
    inProc = !info.hit();
  } else {
    pointIndexHit info = decomposition_().findLine(a, pt);
    if (!info.hit()) {
      inProc = true;
      b = findAnyIntersection.hitPoint();
    } else {
      inProc = true;
      a = findAnyIntersection.hitPoint();
    }
  }
  return inProc;
}


void mousse::conformalVoronoiMesh::dualCellLargestSurfaceProtrusion
(
  const Delaunay::Finite_vertices_iterator& vit,
  pointIndexHit& surfHitLargest,
  label& hitSurfaceLargest
) const
{
  // Set no-hit data
  surfHitLargest = pointIndexHit();
  hitSurfaceLargest = -1;
  std::list<Facet> facets;
  finite_incident_facets(vit, std::back_inserter(facets));
  pointFromPoint vert = topoint(vit->point());
  scalar maxProtrusionDistance = maxSurfaceProtrusion(vert);
  for (auto fit = facets.begin();
       fit != facets.end();
       ++fit) {
    Cell_handle c1 = fit->first;
    Cell_handle c2 = fit->first->neighbor(fit->second);
    if (is_infinite(c1) || is_infinite(c2)
        || (!c1->internalOrBoundaryDualVertex()
            || !c2->internalOrBoundaryDualVertex())
        || !c1->real() || !c2->real()) {
      continue;
    }
    mousse::point endPt = c1->dual();
    if (magSqr(vert - c1->dual()) < magSqr(vert - c2->dual())) {
      endPt = c2->dual();
    }
    if (magSqr(vert - endPt)
        > magSqr(geometryToConformTo().globalBounds().mag())) {
      continue;
    }
    pointIndexHit surfHit;
    label hitSurface;
    geometryToConformTo_.findSurfaceNearestIntersection
      (
        vert,
        endPt,
        surfHit,
        hitSurface
      );
    if (surfHit.hit()) {
      vectorField norm{1};
      allGeometry_[hitSurface].getNormal
        (
          List<pointIndexHit>{1, surfHit},
          norm
        );
      const vector& n = norm[0];
      const scalar normalProtrusionDistance
      {
        (endPt - surfHit.hitPoint()) & n
      };
      if (normalProtrusionDistance > maxProtrusionDistance) {
        const plane p{surfHit.hitPoint(), n};
        const plane::ray r{endPt, -n};
        const scalar d = p.normalIntersect(r);
        mousse::point newPoint = endPt - d*n;
        pointIndexHitAndFeature info;
        geometryToConformTo_.findSurfaceNearest
          (
            newPoint,
            4.0*magSqr(newPoint - endPt),
            info.first(),
            info.second()
          );
        if (info.first().hit()) {
          if (surfaceLocationConformsToInside
              (
                pointIndexHitAndFeature(info.first(), info.second())
              )) {
            surfHitLargest = info.first();
            hitSurfaceLargest = info.second();
            maxProtrusionDistance = normalProtrusionDistance;
          }
        }
      }
    }
  }
  // Relying on short-circuit evaluation to not call for hitPoint when this
  // is a miss
  if (surfHitLargest.hit()
      && (Pstream::parRun()
          && !decomposition().positionOnThisProcessor(surfHitLargest.hitPoint()))) {
    // A protrusion was identified, but not penetrating on this processor,
    // so set no-hit data and allow the other that should have this point
    // referred to generate it.
    surfHitLargest = pointIndexHit();
    hitSurfaceLargest = -1;
  }
}


void mousse::conformalVoronoiMesh::dualCellLargestSurfaceIncursion
(
  const Delaunay::Finite_vertices_iterator& vit,
  pointIndexHit& surfHitLargest,
  label& hitSurfaceLargest
) const
{
  // Set no-hit data
  surfHitLargest = pointIndexHit();
  hitSurfaceLargest = -1;
  std::list<Facet> facets;
  finite_incident_facets(vit, std::back_inserter(facets));
  pointFromPoint vert = topoint(vit->point());
  scalar minIncursionDistance = -maxSurfaceProtrusion(vert);
  for (auto fit = facets.begin();
       fit != facets.end();
       ++fit) {
    Cell_handle c1 = fit->first;
    Cell_handle c2 = fit->first->neighbor(fit->second);
    if (is_infinite(c1) || is_infinite(c2)
        || (!c1->internalOrBoundaryDualVertex()
            || !c2->internalOrBoundaryDualVertex())
        || !c1->real() || !c2->real()) {
      continue;
    }
    mousse::point endPt = c1->dual();
    if (magSqr(vert - c1->dual()) < magSqr(vert - c2->dual())) {
      endPt = c2->dual();
    }
    if (magSqr(vert - endPt)
        > magSqr(geometryToConformTo().globalBounds().mag())) {
      continue;
    }
    pointIndexHit surfHit;
    label hitSurface;
    geometryToConformTo_.findSurfaceNearestIntersection
      (
        vert,
        endPt,
        surfHit,
        hitSurface
      );
    if (!surfHit.hit())
      continue;
    vectorField norm{1};
    allGeometry_[hitSurface].getNormal(List<pointIndexHit>{1, surfHit}, norm);
    const vector& n = norm[0];
    scalar normalIncursionDistance{(endPt - surfHit.hitPoint()) & n};
    if (normalIncursionDistance >= minIncursionDistance)
      continue;
    const plane p{surfHit.hitPoint(), n};
    const plane::ray r{endPt, n};
    const scalar d = p.normalIntersect(r);
    mousse::point newPoint = endPt + d*n;
    pointIndexHitAndFeature info;
    geometryToConformTo_.findSurfaceNearest
      (
        newPoint,
        4.0*magSqr(newPoint - endPt),
        info.first(),
        info.second()
      );
    if (!info.first().hit())
      continue;
    if (surfaceLocationConformsToInside
        (
          pointIndexHitAndFeature(info.first(), info.second())
        )) {
      surfHitLargest = info.first();
      hitSurfaceLargest = info.second();
      minIncursionDistance = normalIncursionDistance;
    }
  }
  // Relying on short-circuit evaluation to not call for hitPoint when this
  // is a miss
  const auto& hP = surfHitLargest.hitPoint();
  if (surfHitLargest.hit()
      && (Pstream::parRun()
          && !decomposition().positionOnThisProcessor(hP))) {
    // A protrusion was identified, but not penetrating on this processor,
    // so set no-hit data and allow the other that should have this point
    // referred to generate it.
    surfHitLargest = pointIndexHit();
    hitSurfaceLargest = -1;
  }
}


void mousse::conformalVoronoiMesh::reportProcessorOccupancy()
{
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       vit++) {
    if (!vit->real())
      continue;
    if (Pstream::parRun()
        && !decomposition().positionOnThisProcessor(topoint(vit->point()))) {
      Pout << topoint(vit->point()) << " is not on this processor " << endl;
    }
  }
}


void mousse::conformalVoronoiMesh::limitDisplacement
(
  const Delaunay::Finite_vertices_iterator& vit,
  vector& displacement,
  label callCount
) const
{
  callCount++;
  // Do not allow infinite recursion
  if (callCount > 7) {
    displacement = vector::zero;
    return;
  }
  pointFromPoint pt = topoint(vit->point());
  mousse::point dispPt = pt + displacement;
  bool limit = false;
  pointIndexHit surfHit;
  label hitSurface;
  if (!geometryToConformTo_.globalBounds().contains(dispPt)) {
    // If dispPt is outside bounding box then displacement cuts boundary
    limit = true;
  } else if (geometryToConformTo_.findSurfaceAnyIntersection(pt, dispPt)) {
    // Full surface penetration test
    limit = true;
  } else {
    // Testing if the displaced position is too close to the surface.
    // Within twice the local surface point pair insertion distance is
    // considered "too close"
    scalar searchDistanceSqr =
      sqr
      (
        2*vit->targetCellSize()*foamyHexMeshControls().pointPairDistanceCoeff()
      );
    geometryToConformTo_.findSurfaceNearest
      (
        dispPt,
        searchDistanceSqr,
        surfHit,
        hitSurface
      );
    if (surfHit.hit()) {
      limit = true;
      if (magSqr(pt - surfHit.hitPoint()) <= searchDistanceSqr) {
        // Cannot limit displacement, point closer than tolerance
        displacement = vector::zero;
        return;
      }
    }
  }
  if (limit) {
    // Halve the displacement and call this function again.  Will continue
    // recursively until the displacement is small enough.
    displacement *= 0.5;
    limitDisplacement(vit, displacement, callCount);
  }
}


mousse::scalar mousse::conformalVoronoiMesh::angleBetweenSurfacePoints
(
  mousse::point pA,
  mousse::point pB
) const
{
  pointIndexHit pAhit;
  label pAsurfaceHit = -1;
  const scalar searchDist = 5.0*targetCellSize(pA);
  geometryToConformTo_.findSurfaceNearest
    (
      pA,
      searchDist,
      pAhit,
      pAsurfaceHit
    );
  if (!pAhit.hit()) {
    return constant::mathematical::pi;
  }
  vectorField norm{1};
  allGeometry_[pAsurfaceHit].getNormal
    (
      List<pointIndexHit>(1, pAhit),
      norm
    );
  const vector nA = norm[0];
  pointIndexHit pBhit;
  label pBsurfaceHit = -1;
  geometryToConformTo_.findSurfaceNearest
    (
      pB,
      searchDist,
      pBhit,
      pBsurfaceHit
    );
  if (!pBhit.hit()) {
    return constant::mathematical::pi;
  }
  allGeometry_[pBsurfaceHit].getNormal
    (
      List<pointIndexHit>(1, pBhit),
      norm
    );
  const vector nB = norm[0];
  return vectorTools::cosPhi(nA, nB);
}


bool mousse::conformalVoronoiMesh::nearSurfacePoint
(
  pointIndexHitAndFeature& pHit
) const
{
  const mousse::point& pt = pHit.first().hitPoint();
  pointIndexHit closePoint;
  const bool closeToSurfacePt = pointIsNearSurfaceLocation(pt, closePoint);
  if (closeToSurfacePt
      && (magSqr(pt - closePoint.hitPoint()) > sqr(pointPairDistance(pt)))) {
    const scalar cosAngle =
      angleBetweenSurfacePoints(pt, closePoint.hitPoint());
    // @todo make this tolerance run-time selectable?
    if (cosAngle < searchAngleOppositeSurface) {
      pointIndexHit pCloseHit;
      label pCloseSurfaceHit = -1;
      const scalar searchDist = targetCellSize(closePoint.hitPoint());
      geometryToConformTo_.findSurfaceNearest
        (
          closePoint.hitPoint(),
          searchDist,
          pCloseHit,
          pCloseSurfaceHit
        );
      vectorField norm{1};
      allGeometry_[pCloseSurfaceHit].getNormal
        (
          List<pointIndexHit>(1, pCloseHit),
          norm
        );
      const vector& nA = norm[0];
      pointIndexHit oppositeHit;
      label oppositeSurfaceHit = -1;
      geometryToConformTo_.findSurfaceNearestIntersection
        (
          closePoint.hitPoint() + 0.5*pointPairDistance(pt)*nA,
          closePoint.hitPoint() + 5*targetCellSize(pt)*nA,
          oppositeHit,
          oppositeSurfaceHit
        );
      if (oppositeHit.hit()) {
        // Replace point
        pHit.first() = oppositeHit;
        pHit.second() = oppositeSurfaceHit;
        return !closeToSurfacePt;
      }
    }
  }
  return closeToSurfacePt;
}


bool mousse::conformalVoronoiMesh::appendToSurfacePtTree
(
  const mousse::point& pt
) const
{
  label startIndex = existingSurfacePtLocations_.size();
  existingSurfacePtLocations_.append(pt);
  label endIndex = existingSurfacePtLocations_.size();
  return surfacePtLocationTreePtr_().insert(startIndex, endIndex);
}


bool mousse::conformalVoronoiMesh::appendToEdgeLocationTree
(
  const mousse::point& pt
) const
{
  label startIndex = existingEdgeLocations_.size();
  existingEdgeLocations_.append(pt);
  label endIndex = existingEdgeLocations_.size();
  return edgeLocationTreePtr_().insert(startIndex, endIndex);
}


mousse::List<mousse::pointIndexHit>
mousse::conformalVoronoiMesh::nearestFeatureEdgeLocations
(
  const mousse::point& pt
) const
{
  const scalar exclusionRangeSqr = featureEdgeExclusionDistanceSqr(pt);
  labelList elems
    = edgeLocationTreePtr_().findSphere(pt, exclusionRangeSqr);
  DynamicList<pointIndexHit> dynPointHit;
  FOR_ALL(elems, elemI) {
    label index = elems[elemI];
    const mousse::point& pointI =
      edgeLocationTreePtr_().shapes().shapePoints()[index];
    pointIndexHit nearHit(true, pointI, index);
    dynPointHit.append(nearHit);
  }
  return dynPointHit;
}


bool mousse::conformalVoronoiMesh::pointIsNearFeatureEdgeLocation
(
  const mousse::point& pt
) const
{
  const scalar exclusionRangeSqr = featureEdgeExclusionDistanceSqr(pt);
  pointIndexHit info =
    edgeLocationTreePtr_().findNearest(pt, exclusionRangeSqr);
  return info.hit();
}


bool mousse::conformalVoronoiMesh::pointIsNearFeatureEdgeLocation
(
  const mousse::point& pt,
  pointIndexHit& info
) const
{
  const scalar exclusionRangeSqr = featureEdgeExclusionDistanceSqr(pt);
  info = edgeLocationTreePtr_().findNearest(pt, exclusionRangeSqr);
  return info.hit();
}


bool mousse::conformalVoronoiMesh::pointIsNearSurfaceLocation
(
  const mousse::point& pt
) const
{
  pointIndexHit info;
  pointIsNearSurfaceLocation(pt, info);
  return info.hit();
}


bool mousse::conformalVoronoiMesh::pointIsNearSurfaceLocation
(
  const mousse::point& pt,
  pointIndexHit& info
) const
{
  const scalar exclusionRangeSqr = surfacePtExclusionDistanceSqr(pt);
  info = surfacePtLocationTreePtr_().findNearest(pt, exclusionRangeSqr);
  return info.hit();
}


bool mousse::conformalVoronoiMesh::nearFeatureEdgeLocation
(
  const pointIndexHit& pHit,
  pointIndexHit& nearestEdgeHit
) const
{
  const mousse::point& pt = pHit.hitPoint();
  const scalar exclusionRangeSqr = featureEdgeExclusionDistanceSqr(pt);
  bool closeToFeatureEdge =
    pointIsNearFeatureEdgeLocation(pt, nearestEdgeHit);
  if (closeToFeatureEdge) {
    List<pointIndexHit> nearHits = nearestFeatureEdgeLocations(pt);
    FOR_ALL(nearHits, elemI) {
      pointIndexHit& info = nearHits[elemI];
      // Check if the edge location that the new edge location is near to
      // "might" be on a different edge. If so, add it anyway.
      pointIndexHit edgeHit;
      label featureHit = -1;
      geometryToConformTo_.findEdgeNearest
        (
          pt,
          exclusionRangeSqr,
          edgeHit,
          featureHit
        );
      const extendedFeatureEdgeMesh& eMesh =
        geometryToConformTo_.features()[featureHit];
      const vector& edgeDir = eMesh.edgeDirections()[edgeHit.index()];
      const vector lineBetweenPoints = pt - info.hitPoint();
      const scalar cosAngle =
        vectorTools::cosPhi(edgeDir, lineBetweenPoints);
      if (mag(cosAngle) < searchConeAngle
          && (mag(lineBetweenPoints) > pointPairDistance(pt))) {
        closeToFeatureEdge = false;
      } else {
        closeToFeatureEdge = true;
        break;
      }
    }
  }
  return closeToFeatureEdge;
}


void mousse::conformalVoronoiMesh::buildEdgeLocationTree
(
  const DynamicList<mousse::point>& existingEdgeLocations
) const
{
  treeBoundBox overallBb
  {
    geometryToConformTo_.globalBounds().extend(rndGen_, 1e-4)
  };
  overallBb.min() -= mousse::point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  overallBb.max() += mousse::point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  edgeLocationTreePtr_.reset
  (
    new dynamicIndexedOctree<dynamicTreeDataPoint>
    {
      dynamicTreeDataPoint{existingEdgeLocations},
      overallBb,  // overall search domain
      10,         // max levels, n/a
      20.0,       // maximum ratio of cubes v.s. cells
      100.0       // max. duplicity; n/a since no bounding boxes.
    }
  );
}


void mousse::conformalVoronoiMesh::buildSurfacePtLocationTree
(
  const DynamicList<mousse::point>& existingSurfacePtLocations
) const
{
  treeBoundBox overallBb
  {
    geometryToConformTo_.globalBounds().extend(rndGen_, 1e-4)
  };
  overallBb.min() -= mousse::point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  overallBb.max() += mousse::point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
  surfacePtLocationTreePtr_.reset
  (
    new dynamicIndexedOctree<dynamicTreeDataPoint>
    {
      dynamicTreeDataPoint{existingSurfacePtLocations},
      overallBb,  // overall search domain
      10,         // max levels, n/a
      20.0,       // maximum ratio of cubes v.s. cells
      100.0       // max. duplicity; n/a since no bounding boxes.
    }
  );
}


void mousse::conformalVoronoiMesh::addSurfaceAndEdgeHits
(
  const mousse::point& vit,
  const pointIndexHitAndFeatureDynList& surfaceIntersections,
  scalar surfacePtReplaceDistCoeffSqr,
  scalar edgeSearchDistCoeffSqr,
  pointIndexHitAndFeatureDynList& surfaceHits,
  pointIndexHitAndFeatureDynList& featureEdgeHits,
  DynamicList<label>& surfaceToTreeShape,
  DynamicList<label>& edgeToTreeShape,
  Map<scalar>& surfacePtToEdgePtDist,
  bool firstPass
) const
{
  const scalar cellSize = targetCellSize(vit);
  const scalar cellSizeSqr = sqr(cellSize);
  FOR_ALL(surfaceIntersections, sI) {
    pointIndexHitAndFeature surfHitI = surfaceIntersections[sI];
    bool keepSurfacePoint = true;
    if (!surfHitI.first().hit()) {
      continue;
    }
    const mousse::point& surfPt = surfHitI.first().hitPoint();
    bool isNearFeaturePt = nearFeaturePt(surfPt);
    bool isNearFeatureEdge = surfacePtNearFeatureEdge(surfPt);
    bool isNearSurfacePt = nearSurfacePoint(surfHitI);
    if (isNearFeaturePt || isNearSurfacePt || isNearFeatureEdge) {
      keepSurfacePoint = false;
    }
    List<List<pointIndexHit>> edHitsByFeature;
    labelList featuresHit;
    const scalar searchRadiusSqr = edgeSearchDistCoeffSqr*cellSizeSqr;
    geometryToConformTo_.findAllNearestEdges
      (
        surfPt,
        searchRadiusSqr,
        edHitsByFeature,
        featuresHit
      );
    FOR_ALL(edHitsByFeature, i) {
      const label featureHit = featuresHit[i];
      List<pointIndexHit>& edHits = edHitsByFeature[i];
      FOR_ALL(edHits, eHitI) {
        pointIndexHit& edHit = edHits[eHitI];
        if (edHit.hit()) {
          const mousse::point& edPt = edHit.hitPoint();
          if (Pstream::parRun()
              && !decomposition().positionOnThisProcessor(edPt)) {
            // Do not insert
            continue;
          }
          if (!nearFeaturePt(edPt)) {
            if (magSqr(edPt - surfPt)
                < surfacePtReplaceDistCoeffSqr*cellSizeSqr) {
              // If the point is within a given distance of a
              // feature edge, give control to edge control points
              // instead, this will prevent "pits" forming.
              // Allow if different surfaces
              keepSurfacePoint = false;
            }
            pointIndexHit nearestEdgeHit;
            if (!nearFeatureEdgeLocation(edHit, nearestEdgeHit)) {
              appendToEdgeLocationTree(edPt);
              edgeToTreeShape.append
              (
                existingEdgeLocations_.size() - 1
              );
              // Do not place edge control points too close to a
              // feature point or existing edge control points
              featureEdgeHits.append
              (
                pointIndexHitAndFeature(edHit, featureHit)
              );
              surfacePtToEdgePtDist.insert
              (
                existingEdgeLocations_.size() - 1,
                magSqr(edPt - surfPt)
              );
            } else if (firstPass) {
              label hitIndex = nearestEdgeHit.index();
              if (magSqr(edPt - surfPt)
                  < surfacePtToEdgePtDist[hitIndex]) {
                featureEdgeHits[hitIndex] =
                  pointIndexHitAndFeature(edHit, featureHit);
                existingEdgeLocations_[hitIndex] =
                  edHit.hitPoint();
                surfacePtToEdgePtDist[hitIndex] =
                  magSqr(edPt - surfPt);
                // Change edge location in featureEdgeHits
                // remove index from edge tree
                // reinsert new point into tree
                edgeLocationTreePtr_().remove(hitIndex);
                edgeLocationTreePtr_().insert
                (
                  hitIndex,
                  hitIndex + 1
                );
              }
            }
          }
        }
      }
    }
    if (keepSurfacePoint) {
      surfaceHits.append(surfHitI);
      appendToSurfacePtTree(surfPt);
      surfaceToTreeShape.append(existingSurfacePtLocations_.size() - 1);
    }
  }
}


void mousse::conformalVoronoiMesh::storeSurfaceConformation()
{
  Info << nl << "Storing surface conformation" << endl;
  surfaceConformationVertices_.clear();
  // Use a temporary dynamic list to speed up insertion.
  DynamicList<Vb> tempSurfaceVertices
  {
    static_cast<label>(number_of_vertices())/10
  };
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       vit++) {
    // Store points that are not referred, part of a pair, but not feature
    // points
    if (!vit->referred() && vit->boundaryPoint()
        && !vit->featurePoint() && !vit->constrained()) {
      tempSurfaceVertices.append
      (
        Vb
        {
          vit->point(),
          vit->index(),
          vit->type(),
          Pstream::myProcNo()
        }
      );
    }
  }
  tempSurfaceVertices.shrink();
  surfaceConformationVertices_.transfer(tempSurfaceVertices);
  Info << "    Stored "
    << returnReduce(label(surfaceConformationVertices_.size()), sumOp<label>())
    << " vertices" << nl << endl;
}


void mousse::conformalVoronoiMesh::reinsertSurfaceConformation()
{
  Info << nl << "Reinserting stored surface conformation" << endl;
  Map<label> oldToNewIndices =
    insertPointPairs(surfaceConformationVertices_, true, true);
  ptPairs_.reIndex(oldToNewIndices);
  PackedBoolList selectedElems{surfaceConformationVertices_.size(), true};
  FOR_ALL(surfaceConformationVertices_, vI) {
    Vb& v = surfaceConformationVertices_[vI];
    label& vIndex = v.index();
    Map<label>::const_iterator iter = oldToNewIndices.find(vIndex);
    if (iter != oldToNewIndices.end()) {
      const label newIndex = iter();
      if (newIndex != -1) {
        vIndex = newIndex;
      } else {
        selectedElems[vI] = false;
      }
    }
  }
  inplaceSubset<PackedBoolList, List<Vb>>
  (
    selectedElems,
    surfaceConformationVertices_
  );
}

