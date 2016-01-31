// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "volume_type.hpp"
namespace mousse
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(uniform, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(cellSizeFunction, uniform, dictionary);
// Constructors 
uniform::uniform
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
  )
{}
// Member Functions 
bool uniform::sizeLocations
(
  const pointIndexHit& /*hitPt*/,
  const vector& /*n*/,
  pointField& shapePts,
  scalarField& shapeSizes
) const
{
  shapePts.setSize(0);
  shapeSizes.setSize(0);
  return true;
}
bool uniform::cellSize
(
  const point& pt,
  scalar& size
) const
{
  List<pointIndexHit> hits;
  surface_.findNearest
  (
    pointField(1, pt),
    scalarField(1, sqr(GREAT)),
    regionIndices_,
    hits
  );
  const pointIndexHit& hitInfo = hits[0];
  if (hitInfo.hit())
  {
    const point& hitPt = hitInfo.hitPoint();
    const label index = hitInfo.index();
    if (sideMode_ == rmBothsides)
    {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      return true;
    }
    size = 0;
    List<pointIndexHit> closeToSurfaceHits;
    surface_.findNearest
    (
      pointField(1, pt),
      scalarField(1, sqr(snapToSurfaceTol_)),
      regionIndices_,
      closeToSurfaceHits
    );
    const pointIndexHit& closeToSurface = closeToSurfaceHits[0];
    // If the nearest point is essentially on the surface, do not do a
    // getVolumeType calculation, as it will be prone to error.
    if (closeToSurface.hit())
    {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      return true;
    }
    pointField ptF(1, pt);
    List<volumeType> vTL(1);
    surface_.getVolumeType(ptF, vTL);
    bool functionApplied = false;
    if (sideMode_ == smInside
        && vTL[0] == volumeType::INSIDE)
    {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      functionApplied = true;
    }
    else if (sideMode_ == smOutside
             && vTL[0] == volumeType::OUTSIDE)
    {
      size = surfaceCellSizeFunction_().interpolate(hitPt, index);
      functionApplied = true;
    }
    return functionApplied;
  }
  return false;
}
bool uniform::setCellSize
(
  const pointField& /*pts*/
)
{
  return true;
}
}  // namespace mousse
