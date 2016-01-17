// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_distance.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"
#include "tri_surface_fields.hpp"
#include "volume_type.hpp"
namespace mousse
{
// Static Data Members
defineTypeNameAndDebug(linearDistance, 0);
addToRunTimeSelectionTable(cellSizeFunction, linearDistance, dictionary);
// Constructors 
linearDistance::linearDistance
(
  const dictionary& initialPointsDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize,
  const labelList regionIndices
)
:
  cellSizeFunction
  (
    typeName,
    initialPointsDict,
    surface,
    defaultCellSize,
    regionIndices
  ),
  distanceCellSize_
  (
    readScalar(coeffsDict().lookup("distanceCellSizeCoeff"))
   *defaultCellSize
  ),
  distance_
  (
    readScalar(coeffsDict().lookup("distanceCoeff"))*defaultCellSize
  ),
  distanceSqr_(sqr(distance_))
{}
// Private Member Functions 
scalar linearDistance::sizeFunction
(
  const point& pt,
  scalar d,
  label index
) const
{
  const scalar interpolatedSize
    = surfaceCellSizeFunction_().interpolate(pt, index);
  scalar gradient
    = (distanceCellSize_ - interpolatedSize)
     /distance_;
  scalar size = gradient*d + interpolatedSize;
  return size;
}
// Member Functions 
bool linearDistance::sizeLocations
(
  const pointIndexHit& hitPt,
  const vector& n,
  pointField& shapePts,
  scalarField& shapeSizes
) const
{
  const mousse::point& pt = hitPt.hitPoint();
  if (sideMode_ == rmBothsides)
  {
    shapePts.resize(2);
    shapeSizes.resize(2);
    shapePts[0] = pt - n*distance_;
    shapeSizes[0] = distanceCellSize_;
    shapePts[1] = pt + n*distance_;
    shapeSizes[1] = distanceCellSize_;
  }
  else if (sideMode_ == smInside)
  {
    shapePts.resize(1);
    shapeSizes.resize(1);
    shapePts[0] = pt - n*distance_;
    shapeSizes[0] = distanceCellSize_;
  }
  else if (sideMode_ == smOutside)
  {
    shapePts.resize(1);
    shapeSizes.resize(1);
    shapePts[0] = pt + n*distance_;
    shapeSizes[0] = distanceCellSize_;
  }
  return false;
}
bool linearDistance::cellSize(const point& pt, scalar& size) const
{
  size = 0;
  List<pointIndexHit> hits;
  surface_.findNearest
  (
    pointField(1, pt),
    scalarField(1, distanceSqr_),
    regionIndices_,
    hits
  );
  const pointIndexHit& hitInfo = hits[0];
  if (hitInfo.hit())
  {
    const point& hitPt = hitInfo.hitPoint();
    const label hitIndex = hitInfo.index();
    const scalar dist = mag(pt - hitPt);
    if (sideMode_ == rmBothsides)
    {
      size = sizeFunction(hitPt, dist, hitIndex);
      return true;
    }
    // If the nearest point is essentially on the surface, do not do a
    // getVolumeType calculation, as it will be prone to error.
    if (dist < snapToSurfaceTol_)
    {
      size = sizeFunction(hitPt, 0, hitIndex);
      return true;
    }
    pointField ptF(1, pt);
    List<volumeType> vTL;
    surface_.getVolumeType(ptF, vTL);
    bool functionApplied = false;
    if
    (
      sideMode_ == smInside
    && vTL[0] == volumeType::INSIDE
    )
    {
      size = sizeFunction(hitPt, dist, hitIndex);
      functionApplied = true;
    }
    else if
    (
      sideMode_ == smOutside
    && vTL[0] == volumeType::OUTSIDE
    )
    {
      size = sizeFunction(hitPt, dist, hitIndex);
      functionApplied = true;
    }
    return functionApplied;
  }
  return false;
}
bool linearDistance::setCellSize(const pointField& pts)
{
//    labelHashSet surfaceAlreadyHit(surfaceCellSize_.size());
//    forAll(pts, ptI)
//    {
//        const mousse::point& pt = pts[ptI];
//        List<pointIndexHit> hits;
//        surface_.findNearest
//        (
//            pointField(1, pt),
//            scalarField(1, distanceSqr_),
//            regionIndices_,
//            hits
//        );
//        const label surfHitI = hits[0].index();
//        if
//        (
//            hits[0].hit()
//         && !surfaceAlreadyHit.found(surfHitI)
//        )
//        {
//            // Halving cell size is arbitrary
//            surfaceCellSizeFunction_().refineSurfaceSize(surfHitI);
//            surfaceAlreadyHit.insert(surfHitI);
//        }
//    }
//    return true;
  return false;
}
}  // namespace mousse
