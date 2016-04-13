// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_tree_data_point.hpp"
#include "tree_bound_box.hpp"
#include "dynamic_indexed_octree.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dynamicTreeDataPoint, 0);

}

// Constructors 
mousse::dynamicTreeDataPoint::dynamicTreeDataPoint
(
  const DynamicList<point>& points
)
:
  points_{points}
{}


// Member Functions 
const mousse::DynamicList<mousse::point>&
mousse::dynamicTreeDataPoint::shapePoints() const
{
  return points_;
}


//- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
//  Only makes sense for closed surfaces.
mousse::volumeType mousse::dynamicTreeDataPoint::getVolumeType
(
  const dynamicIndexedOctree<dynamicTreeDataPoint>& /*oc*/,
  const point& /*sample*/
) const
{
  return volumeType::UNKNOWN;
}


// Check if any point on shape is inside cubeBb.
bool mousse::dynamicTreeDataPoint::overlaps
(
  const label index,
  const treeBoundBox& cubeBb
) const
{
  return cubeBb.contains(points_[index]);
}


// Check if any point on shape is inside sphere.
bool mousse::dynamicTreeDataPoint::overlaps
(
  const label index,
  const point& centre,
  const scalar radiusSqr
) const
{
  const point& p = points_[index];
  const scalar distSqr = magSqr(p - centre);
  if (distSqr <= radiusSqr)
  {
    return true;
  }
  return false;
}


// Calculate nearest point to sample. Updates (if any) nearestDistSqr, minIndex,
// nearestPoint.
void mousse::dynamicTreeDataPoint::findNearest
(
  const labelUList& indices,
  const point& sample,
  scalar& nearestDistSqr,
  label& minIndex,
  point& nearestPoint
) const
{
  FOR_ALL(indices, i)
  {
    const label index = indices[i];
    const point& pt = points_[index];
    scalar distSqr = magSqr(pt - sample);
    if (distSqr < nearestDistSqr)
    {
      nearestDistSqr = distSqr;
      minIndex = index;
      nearestPoint = pt;
    }
  }
}


//- Calculates nearest (to line) point in shape.
//  Returns point and distance (squared)
void mousse::dynamicTreeDataPoint::findNearest
(
  const labelUList& indices,
  const linePointRef& ln,
  treeBoundBox& tightest,
  label& minIndex,
  point& linePoint,
  point& nearestPoint
) const
{
  // Best so far
  scalar nearestDistSqr = magSqr(linePoint - nearestPoint);
  FOR_ALL(indices, i)
  {
    const label index = indices[i];
    const point& shapePt = points_[index];
    if (tightest.contains(shapePt))
    {
      // Nearest point on line
      pointHit pHit = ln.nearestDist(shapePt);
      scalar distSqr = sqr(pHit.distance());
      if (distSqr < nearestDistSqr)
      {
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
  }
}

