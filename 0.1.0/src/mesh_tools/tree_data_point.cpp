// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_data_point.hpp"
#include "tree_bound_box.hpp"
#include "indexed_octree.hpp"
#include "triangle_funcs.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(treeDataPoint, 0);

}


// Constructors 
mousse::treeDataPoint::treeDataPoint(const pointField& points)
:
  points_{points},
  useSubset_{false}
{}


mousse::treeDataPoint::treeDataPoint
(
  const pointField& points,
  const labelList& pointLabels
)
:
  points_{points},
  pointLabels_{pointLabels},
  useSubset_{true}
{}


mousse::treeDataPoint::findNearestOp::findNearestOp
(
  const indexedOctree<treeDataPoint>& tree
)
:
  tree_{tree}
{}


mousse::treeDataPoint::findIntersectOp::findIntersectOp
(
  const indexedOctree<treeDataPoint>&
)
{}


// Member Functions 
mousse::pointField mousse::treeDataPoint::shapePoints() const
{
  if (useSubset_) {
    return pointField(points_, pointLabels_);
  } else {
    return points_;
  }
}


//- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
//  Only makes sense for closed surfaces.
mousse::volumeType mousse::treeDataPoint::getVolumeType
(
  const indexedOctree<treeDataPoint>&,
  const point& /*sample*/
) const
{
  return volumeType::UNKNOWN;
}


// Check if any point on shape is inside cubeBb.
bool mousse::treeDataPoint::overlaps
(
  const label index,
  const treeBoundBox& cubeBb
) const
{
  label pointI = (useSubset_ ? pointLabels_[index] : index);
  return cubeBb.contains(points_[pointI]);
}


// Check if any point on shape is inside sphere.
bool mousse::treeDataPoint::overlaps
(
  const label index,
  const point& centre,
  const scalar radiusSqr
) const
{
  label pointI = (useSubset_ ? pointLabels_[index] : index);
  return magSqr(points_[pointI] - centre) <= radiusSqr;
}


void mousse::treeDataPoint::findNearestOp::operator()
(
  const labelUList& indices,
  const point& sample,
  scalar& nearestDistSqr,
  label& minIndex,
  point& nearestPoint
) const
{
  const treeDataPoint& shape = tree_.shapes();
  FOR_ALL(indices, i) {
    const label index = indices[i];
    label pointI = shape.useSubset() ? shape.pointLabels()[index] : index;
    const point& pt = shape.points()[pointI];
    scalar distSqr = magSqr(pt - sample);
    if (distSqr < nearestDistSqr) {
      nearestDistSqr = distSqr;
      minIndex = index;
      nearestPoint = pt;
    }
  }
}


void mousse::treeDataPoint::findNearestOp::operator()
(
  const labelUList& indices,
  const linePointRef& ln,
  treeBoundBox& tightest,
  label& minIndex,
  point& linePoint,
  point& nearestPoint
) const
{
  const treeDataPoint& shape = tree_.shapes();
  // Best so far
  scalar nearestDistSqr = GREAT;
  if (minIndex >= 0) {
    nearestDistSqr = magSqr(linePoint - nearestPoint);
  }
  FOR_ALL(indices, i) {
    const label index = indices[i];
    label pointI = shape.useSubset() ? shape.pointLabels()[index] : index;
    const point& shapePt = shape.points()[pointI];
    if (!tightest.contains(shapePt))
      continue;
    // Nearest point on line
    pointHit pHit = ln.nearestDist(shapePt);
    scalar distSqr = sqr(pHit.distance());
    if (distSqr >= nearestDistSqr)
      continue;
    nearestDistSqr = distSqr;
    minIndex = index;
    linePoint = pHit.rawPoint();
    nearestPoint = shapePt;

    {
      point& minPt = tightest.min();
      minPt = min(ln.start(), ln.end());
      minPt.x() -= pHit.distance();
      minPt.y() -= pHit.distance();
      minPt.z() -= pHit.distance();
    }

    {
      point& maxPt = tightest.max();
      maxPt = max(ln.start(), ln.end());
      maxPt.x() += pHit.distance();
      maxPt.y() += pHit.distance();
      maxPt.z() += pHit.distance();
    }
  }
}


bool mousse::treeDataPoint::findIntersectOp::operator()
(
  const label /*index*/,
  const point& /*start*/,
  const point& /*end*/,
  point& /*result*/
) const
{
  NOT_IMPLEMENTED
  (
    "treeDataPoint::intersects(const label, const point&,"
    "const point&, point&)"
  );
  return false;
}
