// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cv_2d.hpp"


// Private Member Functions 
bool mousse::CV2D::dualCellSurfaceIntersection
(
  const Triangulation::Finite_vertices_iterator& vit
) const
{
  Triangulation::Edge_circulator ecStart = incident_edges(vit);
  Triangulation::Edge_circulator ec = ecStart;
  do {
    if (is_infinite(ec))
      continue;
    point e0 = toPoint3D(circumcenter(ec->first));
    // If edge end is outside bounding box then edge cuts boundary
    if (!qSurf_.globalBounds().contains(e0)) {
      return true;
    }
    point e1 = toPoint3D(circumcenter(ec->first->neighbor(ec->second)));
    // If other edge end is ouside bounding box then edge cuts boundary
    if (!qSurf_.globalBounds().contains(e1)) {
      return true;
    }
    if (magSqr(e1 - e0) > meshControls().minEdgeLen2()) {
      if (qSurf_.findSurfaceAnyIntersection(e0, e1)) {
        return true;
      }
    }
  } while (++ec != ecStart);
  return false;
}


void mousse::CV2D::insertPointPairs
(
  const DynamicList<point2D>& nearSurfacePoints,
  const DynamicList<point2D>& surfacePoints,
  const DynamicList<label>& surfaceTris,
  const DynamicList<label>& surfaceHits,
  const fileName fName
)
{
  if (meshControls().mirrorPoints()) {
    FOR_ALL(surfacePoints, ppi) {
      insertMirrorPoint
      (
        nearSurfacePoints[ppi],
        surfacePoints[ppi]
      );
    }
  }
  else {
    FOR_ALL(surfacePoints, ppi) {
      pointIndexHit pHit
      {
        true,
        toPoint3D(surfacePoints[ppi]),
        surfaceTris[ppi]
      };
      vectorField norm{1};
      qSurf_.geometry()[surfaceHits[ppi]].getNormal
      (
        List<pointIndexHit>(1, pHit),
        norm
      );
      insertPointPair
      (
        meshControls().ppDist(),
        surfacePoints[ppi],
        toPoint2D(norm[0])
      );
    }
  }
  Info << surfacePoints.size() << " point-pairs inserted" << endl;
  if (meshControls().objOutput()) {
    OFstream str{fName};
    label vertI = 0;
    FOR_ALL(surfacePoints, ppi) {
      meshTools::writeOBJ(str, toPoint3D(surfacePoints[ppi]));
      vertI++;
    }
    Info << "insertPointPairs: Written " << surfacePoints.size()
      << " inserted point-pair locations to file "
      << str.name() << endl;
  }
}


void mousse::CV2D::insertSurfaceNearestPointPairs()
{
  Info << "insertSurfaceNearestPointPairs: ";
  label nSurfacePointsEst =
    min
    (
      number_of_vertices(),
      size_t(10*sqrt(scalar(number_of_vertices())))
    );
  DynamicList<point2D> nearSurfacePoints{nSurfacePointsEst};
  DynamicList<point2D> surfacePoints{nSurfacePointsEst};
  DynamicList<label> surfaceTris{nSurfacePointsEst};
  DynamicList<label> surfaceHits{nSurfacePointsEst};
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       vit++) {
    if (!vit->internalPoint())
      continue;
    point2DFromPoint vert{toPoint2D(vit->point())};
    pointIndexHit pHit;
    label hitSurface = -1;
    qSurf_.findSurfaceNearest
    (
      toPoint3D(vert),
      4*meshControls().minCellSize2(),
      pHit,
      hitSurface
    );
    if (pHit.hit()) {
      vit->setNearBoundary();
      if (dualCellSurfaceIntersection(vit)) {
        nearSurfacePoints.append(vert);
        surfacePoints.append(toPoint2D(pHit.hitPoint()));
        surfaceTris.append(pHit.index());
        surfaceHits.append(hitSurface);
      }
    }
  }
  insertPointPairs
  (
    nearSurfacePoints,
    surfacePoints,
    surfaceTris,
    surfaceHits,
    "surfaceNearestIntersections.obj"
  );
}

