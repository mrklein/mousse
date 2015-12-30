// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "transform.hpp"
// Constructors 
// Null constructor
inline mousse::pointData::pointData()
:
  pointEdgePoint(),
  s_(GREAT),
  v_(point::max)
{}
// Construct from origin, distance
inline mousse::pointData::pointData
(
  const point& origin,
  const scalar distSqr,
  const scalar s,
  const vector& v
)
:
  pointEdgePoint(origin, distSqr),
  s_(s),
  v_(v)
{}
// Construct as copy
inline mousse::pointData::pointData(const pointData& wpt)
:
  pointEdgePoint(wpt),
  s_(wpt.s()),
  v_(wpt.v())
{}
// Member Functions 
inline mousse::scalar mousse::pointData::s() const
{
  return s_;
}
inline const mousse::vector& mousse::pointData::v() const
{
  return v_;
}
template<class TrackingData>
inline void mousse::pointData::transform
(
  const tensor& rotTensor,
  TrackingData& td
)
{
  pointEdgePoint::transform(rotTensor, td);
  v_ = mousse::transform(rotTensor, v_);
}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const label edgeI,
  const pointData& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  if
  (
    pointEdgePoint::updatePoint
    (
      mesh,
      pointI,
      edgeI,
      edgeInfo,
      tol,
      td
    )
  )
  {
    s_ = edgeInfo.s_;
    v_ = edgeInfo.v_;
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const pointData& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if
  (
    pointEdgePoint::updatePoint
    (
      mesh,
      pointI,
      newPointInfo,
      tol,
      td
    )
  )
  {
    s_ = newPointInfo.s_;
    v_ = newPointInfo.v_;
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const pointData& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (pointEdgePoint::updatePoint(newPointInfo, tol, td))
  {
    s_ = newPointInfo.s_;
    v_ = newPointInfo.v_;
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointData::updateEdge
(
  const polyMesh& mesh,
  const label edgeI,
  const label pointI,
  const pointData& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if
  (
    pointEdgePoint::updateEdge
    (
      mesh,
      edgeI,
      pointI,
      pointInfo,
      tol,
      td
    )
  )
  {
    s_ = pointInfo.s_;
    v_ = pointInfo.v_;
    return true;
  }
  else
  {
    return false;
  }
}
// Member Operators 
inline bool mousse::pointData::operator==(const mousse::pointData& rhs)
const
{
  return
    pointEdgePoint::operator==(rhs)
  && (s() == rhs.s())
  && (v() == rhs.v());
}
inline bool mousse::pointData::operator!=(const mousse::pointData& rhs)
const
{
  return !(*this == rhs);
}
