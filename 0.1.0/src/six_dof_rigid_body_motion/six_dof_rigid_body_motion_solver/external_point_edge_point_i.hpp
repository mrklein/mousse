// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "transform.hpp"
// Private Member Functions 
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::update
(
  const point& pt,
  const externalPointEdgePoint& w2,
  const scalar tol,
  TrackingData& td
)
{
  scalar dist2 = magSqr(pt - w2.origin());
  if (!valid(td))
  {
    // current not yet set so use any value
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - dist2;
  if (diff < 0)
  {
    // already nearer to pt
    return false;
  }
  if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol)))
  {
    // don't propagate small changes
    return false;
  }
  else
  {
    // update with new values
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::update
(
  const externalPointEdgePoint& w2,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td))
  {
    // current not yet set so use any value
    distSqr_ = w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - w2.distSqr();
  if (diff < 0)
  {
    // already nearer to pt
    return false;
  }
  if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol)))
  {
    // don't propagate small changes
    return false;
  }
  else
  {
    // update with new values
    distSqr_ =  w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
}
// Constructors 
inline mousse::externalPointEdgePoint::externalPointEdgePoint()
:
  origin_(point::max),
  distSqr_(GREAT)
{}
inline mousse::externalPointEdgePoint::externalPointEdgePoint
(
  const point& origin,
  const scalar distSqr
)
:
  origin_(origin),
  distSqr_(distSqr)
{}
inline mousse::externalPointEdgePoint::externalPointEdgePoint
(
  const externalPointEdgePoint& wpt
)
:
  origin_(wpt.origin()),
  distSqr_(wpt.distSqr())
{}
// Member Functions 
inline const mousse::point& mousse::externalPointEdgePoint::origin() const
{
  return origin_;
}
inline mousse::scalar mousse::externalPointEdgePoint::distSqr() const
{
  return distSqr_;
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::valid(TrackingData&) const
{
  return origin_ != point::max;
}
// Checks for cyclic points
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::sameGeometry
(
  const externalPointEdgePoint& w2,
  const scalar tol,
  TrackingData&
) const
{
  scalar diff = mousse::mag(distSqr() - w2.distSqr());
  if (diff < SMALL)
  {
    return true;
  }
  else
  {
    if ((distSqr() > SMALL) && ((diff/distSqr()) < tol))
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
inline void mousse::externalPointEdgePoint::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  origin_ -= coord;
}
template<class TrackingData>
inline void mousse::externalPointEdgePoint::transform
(
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
template<class TrackingData>
inline void mousse::externalPointEdgePoint::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  // back to absolute form
  origin_ += coord;
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::updatePoint
(
  const polyMesh&,
  const label pointI,
  const label /*edgeI*/,
  const externalPointEdgePoint& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(td.points_[pointI], edgeInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::updatePoint
(
  const polyMesh&,
  const label pointI,
  const externalPointEdgePoint& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(td.points_[pointI], newPointInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::updatePoint
(
  const externalPointEdgePoint& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(newPointInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::updateEdge
(
  const polyMesh& mesh,
  const label edgeI,
  const label /*pointI*/,
  const externalPointEdgePoint& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  const edge& e = mesh.edges()[edgeI];
  return update(e.centre(td.points_), pointInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::externalPointEdgePoint::equal
(
  const externalPointEdgePoint& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::externalPointEdgePoint::operator==
(
  const mousse::externalPointEdgePoint& rhs
)
const
{
  return (origin() == rhs.origin()) && (distSqr() == rhs.distSqr());
}
inline bool mousse::externalPointEdgePoint::operator!=
(
  const mousse::externalPointEdgePoint& rhs
)
const
{
  return !(*this == rhs);
}
