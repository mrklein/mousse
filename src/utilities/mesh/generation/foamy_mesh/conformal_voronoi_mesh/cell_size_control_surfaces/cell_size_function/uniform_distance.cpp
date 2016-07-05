// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_distance.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "volume_type.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(uniformDistance, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(cellSizeFunction, uniformDistance, dictionary);


// Constructors 
uniformDistance::uniformDistance
(
  const dictionary& initialPointsDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize,
  const labelList regionIndices
)
:
  cellSizeFunction
  {
    typeName,
    initialPointsDict,
    surface,
    defaultCellSize,
    regionIndices
  },
  distance_
  {
    readScalar(coeffsDict().lookup("distanceCoeff"))*defaultCellSize
  },
  distanceSqr_{sqr(distance_)}
{}


// Member Functions 
bool uniformDistance::sizeLocations
(
  const pointIndexHit& hitPt,
  const vector& n,
  pointField& shapePts,
  scalarField& shapeSizes
) const
{
  const mousse::point& pt = hitPt.hitPoint();
  const scalar distanceCellSize =
    surfaceCellSizeFunction_().interpolate(pt, hitPt.index());
  if (sideMode_ == rmBothsides) {
    shapePts.resize(2);
    shapeSizes.resize(2);
    shapePts[0] = pt - n*distance_;
    shapeSizes[0] = distanceCellSize;
    shapePts[1] = pt + n*distance_;
    shapeSizes[1] = distanceCellSize;
  } else if (sideMode_ == smInside) {
    shapePts.resize(1);
    shapeSizes.resize(1);
    shapePts[0] = pt - n*distance_;
    shapeSizes[0] = distanceCellSize;
  } else if (sideMode_ == smOutside) {
    shapePts.resize(1);
    shapeSizes.resize(1);
    shapePts[0] = pt - n*distance_;
    shapeSizes[0] = distanceCellSize;
  }
  return false;
}


bool uniformDistance::cellSize
(
  const point& pt,
  scalar& size
) const
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
  if (hitInfo.hit()) {
    const point& hitPt = hitInfo.hitPoint();
    const label index = hitInfo.index();
    if (sideMode_ == rmBothsides) {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      return true;
    }
    // If the nearest point is essentially on the surface, do not do a
    // getVolumeType calculation, as it will be prone to error.
    if (mag(pt  - hitInfo.hitPoint()) < snapToSurfaceTol_) {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      return true;
    }
    pointField ptF{1, pt};
    List<volumeType> vTL;
    surface_.getVolumeType(ptF, vTL);
    bool functionApplied = false;
    if (sideMode_ == smInside && vTL[0] == volumeType::INSIDE) {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      functionApplied = true;
    } else if (sideMode_ == smOutside && vTL[0] == volumeType::OUTSIDE) {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      functionApplied = true;
    }
    return functionApplied;
  }
  return false;
}


bool uniformDistance::setCellSize
(
  const pointField& /*pts*/
)
{
  return false;
}

}  // namespace mousse

