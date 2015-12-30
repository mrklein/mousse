// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Private Member Functions 
// Constructors 
// Null constructor
inline mousse::topoDistanceData::topoDistanceData()
:
  data_(-1),
  distance_(-1)
{}
// Construct from components
inline mousse::topoDistanceData::topoDistanceData
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
inline bool mousse::topoDistanceData::valid(TrackingData& td) const
{
  return distance_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::topoDistanceData::sameGeometry
(
  const polyMesh&,
  const topoDistanceData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::topoDistanceData::leaveDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::topoDistanceData::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::topoDistanceData::enterDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData&
)
{}
// Update cell with neighbouring face information
template<class TrackingData>
inline bool mousse::topoDistanceData::updateCell
(
  const polyMesh&,
  const label thisCellI,
  const label neighbourFaceI,
  const topoDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(neighbourInfo);
    return true;
  }
  else
  {
    return false;
  }
}
// Update face with neighbouring cell information
template<class TrackingData>
inline bool mousse::topoDistanceData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label neighbourCellI,
  const topoDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData&
)
{
  // From cell to its faces.
  if (distance_ == -1)
  {
    data_ = neighbourInfo.data_;
    distance_ = neighbourInfo.distance_ + 1;
    return true;
  }
  else
  {
    return false;
  }
}
// Update face with coupled face information
template<class TrackingData>
inline bool mousse::topoDistanceData::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const topoDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData&
)
{
  // From face to face (e.g. coupled faces)
  if (distance_ == -1)
  {
    operator=(neighbourInfo);
    return true;
  }
  else
  {
    return false;
  }
}
template<class TrackingData>
inline bool mousse::topoDistanceData::equal
(
  const topoDistanceData& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::topoDistanceData::operator==
(
  const mousse::topoDistanceData& rhs
) const
{
  return data() == rhs.data() && distance() == rhs.distance();
}
inline bool mousse::topoDistanceData::operator!=
(
  const mousse::topoDistanceData& rhs
) const
{
  return !(*this == rhs);
}
