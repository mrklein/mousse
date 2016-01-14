// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "transform.hpp"
// Private Member Functions 
// Update this with w2.
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::update
(
  const pointEdgeStructuredWalk& w2,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!valid(td))
  {
    // current not yet set. Walked from w2 to here (=point0)
    dist_ = w2.dist_ + mag(point0_-w2.previousPoint_);
    previousPoint_ = point0_;
    data_ = w2.data_;
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
// Null constructor
inline mousse::pointEdgeStructuredWalk::pointEdgeStructuredWalk()
:
  point0_(vector::max),
  previousPoint_(vector::max),
  dist_(0),
  data_(vector::zero)
{}
// Construct from origin, distance
inline mousse::pointEdgeStructuredWalk::pointEdgeStructuredWalk
(
  const point& point0,
  const point& previousPoint,
  const scalar dist,
  const vector& data
)
:
  point0_(point0),
  previousPoint_(previousPoint),
  dist_(dist),
  data_(data)
{}
// Member Functions 
inline bool mousse::pointEdgeStructuredWalk::inZone() const
{
  return point0_ != vector::max;
}
//inline const mousse::point& mousse::pointEdgeStructuredWalk::previousPoint() const
//{
//    return previousPoint_;
//}
inline mousse::scalar mousse::pointEdgeStructuredWalk::dist() const
{
  return dist_;
}
inline const mousse::vector& mousse::pointEdgeStructuredWalk::data() const
{
  return data_;
}
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::valid(TrackingData&) const
{
  return previousPoint_ != vector::max;
}
// Checks for cyclic points
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::sameGeometry
(
  const pointEdgeStructuredWalk& w2,
  const scalar tol,
  TrackingData&
) const
{
  scalar diff = mousse::mag(dist() - w2.dist());
  if (diff < SMALL)
  {
    return true;
  }
  else
  {
    if ((dist() > SMALL) && ((diff/dist()) < tol))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}
template<class TrackingData>
inline void mousse::pointEdgeStructuredWalk::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  previousPoint_ -= coord;
}
template<class TrackingData>
inline void mousse::pointEdgeStructuredWalk::transform
(
  const tensor& rotTensor,
  TrackingData&
)
{
  previousPoint_ = mousse::transform(rotTensor, previousPoint_);
}
// Update absolute geometric quantities. Note that distance (dist_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void mousse::pointEdgeStructuredWalk::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  // back to absolute form
  previousPoint_ += coord;
}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const label /*edgeI*/,
  const pointEdgeStructuredWalk& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(edgeInfo, tol, td);
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const pointEdgeStructuredWalk& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(newPointInfo, tol, td);
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const pointEdgeStructuredWalk& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(newPointInfo, tol, td);
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updateEdge
(
  const polyMesh&,
  const label /*edgeI*/,
  const label /*pointI*/,
  const pointEdgeStructuredWalk& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(pointInfo, tol, td);
  }
  else
  {
    return false;
  }
}
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::equal
(
  const pointEdgeStructuredWalk& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::pointEdgeStructuredWalk::operator==
(
  const mousse::pointEdgeStructuredWalk& rhs
) const
{
  return previousPoint_ == rhs.previousPoint_;
}
inline bool mousse::pointEdgeStructuredWalk::operator!=
(
  const mousse::pointEdgeStructuredWalk& rhs
) const
{
  return !(*this == rhs);
}
