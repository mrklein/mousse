// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_data_edge.hpp"
#include "indexed_octree.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(treeDataEdge, 0);
}
// Private Member Functions 
mousse::treeBoundBox mousse::treeDataEdge::calcBb(const label edgeI) const
{
  const edge& e = edges_[edgeI];
  const point& p0 = points_[e[0]];
  const point& p1 = points_[e[1]];
  return treeBoundBox(min(p0, p1), max(p0, p1));
}
void mousse::treeDataEdge::update()
{
  if (cacheBb_)
  {
    bbs_.setSize(edgeLabels_.size());
    forAll(edgeLabels_, i)
    {
      bbs_[i] = calcBb(edgeLabels_[i]);
    }
  }
}
// Constructors 
mousse::treeDataEdge::treeDataEdge
(
  const bool cacheBb,
  const edgeList& edges,
  const pointField& points,
  const labelUList& edgeLabels
)
:
  edges_(edges),
  points_(points),
  edgeLabels_(edgeLabels),
  cacheBb_(cacheBb)
{
  update();
}
mousse::treeDataEdge::treeDataEdge
(
  const bool cacheBb,
  const edgeList& edges,
  const pointField& points,
  const Xfer<labelList>& edgeLabels
)
:
  edges_(edges),
  points_(points),
  edgeLabels_(edgeLabels),
  cacheBb_(cacheBb)
{
  update();
}
mousse::treeDataEdge::findNearestOp::findNearestOp
(
  const indexedOctree<treeDataEdge>& tree
)
:
  tree_(tree)
{}
mousse::treeDataEdge::findIntersectOp::findIntersectOp
(
  const indexedOctree<treeDataEdge>& tree
)
{}
// Member Functions 
mousse::pointField mousse::treeDataEdge::shapePoints() const
{
  pointField eMids(edgeLabels_.size());
  forAll(edgeLabels_, i)
  {
    const edge& e = edges_[edgeLabels_[i]];
    eMids[i] = e.centre(points_);
  }
  return eMids;
}
//- Get type (inside,outside,mixed,unknown) of point w.r.t. surface.
//  Only makes sense for closed surfaces.
mousse::volumeType mousse::treeDataEdge::getVolumeType
(
  const indexedOctree<treeDataEdge>& oc,
  const point& sample
) const
{
  return volumeType::UNKNOWN;
}
// Check if any point on shape is inside cubeBb.
bool mousse::treeDataEdge::overlaps
(
  const label index,
  const treeBoundBox& cubeBb
) const
{
  const edge& e = edges_[edgeLabels_[index]];
  const point& start = points_[e.start()];
  const point& end = points_[e.end()];
  point intersect;
  return cubeBb.intersects(start, end, intersect);
}
// Check if any point on shape is inside sphere.
bool mousse::treeDataEdge::overlaps
(
  const label index,
  const point& centre,
  const scalar radiusSqr
) const
{
  const edge& e = edges_[edgeLabels_[index]];
  const pointHit nearHit = e.line(points_).nearestDist(centre);
  const scalar distSqr = sqr(nearHit.distance());
  if (distSqr <= radiusSqr)
  {
    return true;
  }
  return false;
}
void mousse::treeDataEdge::findNearestOp::operator()
(
  const labelUList& indices,
  const point& sample,
  scalar& nearestDistSqr,
  label& minIndex,
  point& nearestPoint
) const
{
  const treeDataEdge& shape = tree_.shapes();
  forAll(indices, i)
  {
    const label index = indices[i];
    const edge& e = shape.edges()[shape.edgeLabels()[index]];
    pointHit nearHit = e.line(shape.points()).nearestDist(sample);
    scalar distSqr = sqr(nearHit.distance());
    if (distSqr < nearestDistSqr)
    {
      nearestDistSqr = distSqr;
      minIndex = index;
      nearestPoint = nearHit.rawPoint();
    }
  }
}
void mousse::treeDataEdge::findNearestOp::operator()
(
  const labelUList& indices,
  const linePointRef& ln,
  treeBoundBox& tightest,
  label& minIndex,
  point& linePoint,
  point& nearestPoint
) const
{
  const treeDataEdge& shape = tree_.shapes();
  // Best so far
  scalar nearestDistSqr = magSqr(linePoint - nearestPoint);
  forAll(indices, i)
  {
    const label index = indices[i];
    const edge& e = shape.edges()[shape.edgeLabels()[index]];
    // Note: could do bb test ? Worthwhile?
    // Nearest point on line
    point ePoint, lnPt;
    scalar dist = e.line(shape.points()).nearestDist(ln, ePoint, lnPt);
    scalar distSqr = sqr(dist);
    if (distSqr < nearestDistSqr)
    {
      nearestDistSqr = distSqr;
      minIndex = index;
      linePoint = lnPt;
      nearestPoint = ePoint;
      {
        point& minPt = tightest.min();
        minPt = min(ln.start(), ln.end());
        minPt.x() -= dist;
        minPt.y() -= dist;
        minPt.z() -= dist;
      }
      {
        point& maxPt = tightest.max();
        maxPt = max(ln.start(), ln.end());
        maxPt.x() += dist;
        maxPt.y() += dist;
        maxPt.z() += dist;
      }
    }
  }
}
bool mousse::treeDataEdge::findIntersectOp::operator()
(
  const label index,
  const point& start,
  const point& end,
  point& result
) const
{
  notImplemented
  (
    "treeDataEdge::intersects(const label, const point&,"
    "const point&, point&)"
  );
  return false;
}
