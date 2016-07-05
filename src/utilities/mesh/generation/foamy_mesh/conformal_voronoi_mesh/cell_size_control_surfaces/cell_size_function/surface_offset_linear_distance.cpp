// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_offset_linear_distance.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "volume_type.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(surfaceOffsetLinearDistance, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  cellSizeFunction,
  surfaceOffsetLinearDistance,
  dictionary
);


// Constructors 
surfaceOffsetLinearDistance::surfaceOffsetLinearDistance
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
  distanceCellSize_
  {
    readScalar(coeffsDict().lookup("distanceCellSizeCoeff"))*defaultCellSize
  },
  surfaceOffset_
  {
    readScalar(coeffsDict().lookup("surfaceOffsetCoeff"))*defaultCellSize
  },
  totalDistance_{},
  totalDistanceSqr_{}
{
  if (coeffsDict().found("totalDistanceCoeff")
      || coeffsDict().found("linearDistanceCoeff")) {
    if (coeffsDict().found("totalDistanceCoeff")
        && coeffsDict().found("linearDistanceCoeff")) {
      FATAL_ERROR_IN
      (
        "surfaceOffsetLinearDistance::surfaceOffsetLinearDistance"
        "("
        "  const dictionary& initialPointsDict,"
        "  const searchableSurface& surface,"
        "  const scalar& defaultCellSize"
        ")"
      )
      << "totalDistanceCoeff and linearDistanceCoeff found, "
      << "specify one or other, not both."
      << nl << exit(FatalError) << endl;
    }
    if (coeffsDict().found("totalDistanceCoeff")) {
      totalDistance_ =
        readScalar(coeffsDict().lookup("totalDistanceCoeff"))*defaultCellSize;
    } else {
      totalDistance_ =
        readScalar(coeffsDict().lookup("linearDistanceCoeff"))*defaultCellSize
        + surfaceOffset_;
    }
  } else {
    FATAL_ERROR_IN
    (
      "surfaceOffsetLinearDistance::surfaceOffsetLinearDistance"
      "("
      "  const dictionary& initialPointsDict,"
      "  const searchableSurface& surface,"
      "  const scalar& defaultCellSize"
      ")"
    )
    << "totalDistanceCoeff or linearDistanceCoeff not found."
    << nl << exit(FatalError) << endl;
  }
  totalDistanceSqr_ = sqr(totalDistance_);
}


// Private Member Functions 
scalar surfaceOffsetLinearDistance::sizeFunction
(
  const point& pt,
  scalar d,
  label index
) const
{
  const scalar interpolatedSize
    = surfaceCellSizeFunction_().interpolate(pt, index);
  if (d <= surfaceOffset_) {
    return interpolatedSize;
  }
  scalar gradient =
    (distanceCellSize_ - interpolatedSize)/(totalDistance_ - surfaceOffset_);
  scalar intercept = interpolatedSize - gradient*surfaceOffset_;
  return gradient*d + intercept;
}


// Member Functions 
bool surfaceOffsetLinearDistance::sizeLocations
(
  const pointIndexHit& hitPt,
  const vector& n,
  pointField& shapePts,
  scalarField& shapeSizes
) const
{
  const mousse::point& pt = hitPt.hitPoint();
  const scalar offsetCellSize =
    surfaceCellSizeFunction_().interpolate(pt, hitPt.index());
  if (sideMode_ == rmBothsides) {
    shapePts.resize(4);
    shapeSizes.resize(4);
    shapePts[0] = pt - n*surfaceOffset_;
    shapeSizes[0] = offsetCellSize;
    shapePts[1] = pt - n*totalDistance_;
    shapeSizes[1] = distanceCellSize_;
    shapePts[2] = pt + n*surfaceOffset_;
    shapeSizes[2] = offsetCellSize;
    shapePts[3] = pt + n*totalDistance_;
    shapeSizes[3] = distanceCellSize_;
  } else if (sideMode_ == smInside) {
    shapePts.resize(2);
    shapeSizes.resize(2);
    shapePts[0] = pt - n*surfaceOffset_;
    shapeSizes[0] = offsetCellSize;
    shapePts[1] = pt - n*totalDistance_;
    shapeSizes[1] = distanceCellSize_;
  } else if (sideMode_ == smOutside) {
    shapePts.resize(2);
    shapeSizes.resize(2);
    shapePts[0] = pt + n*surfaceOffset_;
    shapeSizes[0] = offsetCellSize;
    shapePts[1] = pt + n*totalDistance_;
    shapeSizes[1] = distanceCellSize_;
  }
  return true;
}


bool surfaceOffsetLinearDistance::cellSize
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
      scalarField(1, totalDistanceSqr_),
      regionIndices_,
      hits
    );
  const pointIndexHit& hitInfo = hits[0];
  if (hitInfo.hit()) {
    const point& hitPt = hitInfo.hitPoint();
    const label hitIndex = hitInfo.index();
    const scalar dist = mag(pt - hitPt);
    if (sideMode_ == rmBothsides) {
      size = sizeFunction(hitPt, dist, hitIndex);
      return true;
    }
    // If the nearest point is essentially on the surface, do not do a
    // getVolumeType calculation, as it will be prone to error.
    if (mag(pt  - hitInfo.hitPoint()) < snapToSurfaceTol_) {
      size = sizeFunction(hitPt, 0, hitIndex);
      return true;
    }
    pointField ptF{1, pt};
    List<volumeType> vTL;
    surface_.getVolumeType(ptF, vTL);
    bool functionApplied = false;
    if (sideMode_ == smInside && vTL[0] == volumeType::INSIDE) {
      size = sizeFunction(hitPt, dist, hitIndex);
      functionApplied = true;
    } else if (sideMode_ == smOutside && vTL[0] == volumeType::OUTSIDE) {
      size = sizeFunction(hitPt, dist, hitIndex);
      functionApplied = true;
    }
    return functionApplied;
  }
  return false;
}

}  // namespace mousse

