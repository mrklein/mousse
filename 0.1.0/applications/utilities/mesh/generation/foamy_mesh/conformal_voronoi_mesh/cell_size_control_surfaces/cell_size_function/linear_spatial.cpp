// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear_spatial.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "volume_type.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(linearSpatial, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(cellSizeFunction, linearSpatial, dictionary);


// Constructors 
linearSpatial::linearSpatial
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
  referencePoint_{coeffsDict().lookup("referencePoint")},
  referenceCellSize_
  {
    readScalar(coeffsDict().lookup("referenceCellSizeCoeff"))*defaultCellSize
  },
  direction_{coeffsDict().lookup("direction")},
  cellSizeGradient_{readScalar(coeffsDict().lookup("cellSizeGradient"))}
{
  direction_ /= mag(direction_);
}


// Private Member Functions 
scalar linearSpatial::sizeFunction(const point& pt) const
{
  return
    referenceCellSize_
    + ((pt - referencePoint_) & direction_)*cellSizeGradient_;
}


// Member Functions 
bool linearSpatial::sizeLocations
(
  const pointIndexHit& /*hitPt*/,
  const vector& /*n*/,
  pointField& /*shapePts*/,
  scalarField& /*shapeSizes*/
) const
{
  if (sideMode_ == rmBothsides) {
  } else if (sideMode_ == smInside) {
  } else if (sideMode_ == smOutside) {
  }
  return false;
}


bool linearSpatial::cellSize
(
  const point& pt,
  scalar& size
) const
{
  if (sideMode_ == rmBothsides) {
    size = sizeFunction(pt);
    return true;
  }
  size = 0;
  List<pointIndexHit> hits;
  surface_.findNearest
    (
      pointField(1, pt),
      scalarField(1, sqr(snapToSurfaceTol_)),
      regionIndices_,
      hits
    );
  const pointIndexHit& hitInfo = hits[0];
  // If the nearest point is essentially on the surface, do not do a
  // getVolumeType calculation, as it will be prone to error.
  if (hitInfo.hit()) {
    size = sizeFunction(pt);
    return true;
  }
  pointField ptF{1, pt};
  List<volumeType> vTL;
  surface_.getVolumeType(ptF, vTL);
  bool functionApplied = false;
  if (sideMode_ == smInside && vTL[0] == volumeType::INSIDE) {
    size = sizeFunction(pt);
    functionApplied = true;
  } else if (sideMode_ == smOutside && vTL[0] == volumeType::OUTSIDE) {
    size = sizeFunction(pt);
    functionApplied = true;
  }
  return functionApplied;
}

}  // namespace mousse

