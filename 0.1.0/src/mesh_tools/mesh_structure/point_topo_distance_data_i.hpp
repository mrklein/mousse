// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Private Member Functions 
// Constructors 
// Null constructor
inline mousse::pointTopoDistanceData::pointTopoDistanceData()
:
  data_(-1),
  distance_(-1)
{}
// Construct from components
inline mousse::pointTopoDistanceData::pointTopoDistanceData
(
  const label data,
  const label distance
)
:
  data_(data),
  distance_(distance)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::valid(TrackingData&) const
{
  return distance_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::sameGeometry
(
  const pointTopoDistanceData&,
  const scalar /*tol*/,
  TrackingData&
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& /*coord*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::transform
(
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& /*coord*/,
  TrackingData&
)
{}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const label /*edgeI*/,
  const pointTopoDistanceData& edgeInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    data_ = edgeInfo.data_;
    distance_ = edgeInfo.distance_ + 1;
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const pointTopoDistanceData& newPointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(newPointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const pointTopoDistanceData& newPointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(newPointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updateEdge
(
  const polyMesh&,
  const label /*edgeI*/,
  const label /*pointI*/,
  const pointTopoDistanceData& pointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(pointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::equal
(
  const pointTopoDistanceData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::pointTopoDistanceData::operator==
(
  const mousse::pointTopoDistanceData& rhs
) const
{
  return data() == rhs.data() && distance() == rhs.distance();
}
inline bool mousse::pointTopoDistanceData::operator!=
(
  const mousse::pointTopoDistanceData& rhs
) const
{
  return !(*this == rhs);
}
