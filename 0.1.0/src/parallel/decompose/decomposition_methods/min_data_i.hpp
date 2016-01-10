// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Constructors 
inline mousse::minData::minData()
:
  data_(labelMax)
{}
inline mousse::minData::minData(const label data)
:
  data_(data)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::minData::valid(TrackingData&) const
{
  return data_ != labelMax;
}
template<class TrackingData>
inline bool mousse::minData::sameGeometry
(
  const polyMesh&,
  const minData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::minData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}
template<class TrackingData>
inline void mousse::minData::transform
(
  const polyMesh&,
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}
template<class TrackingData>
inline void mousse::minData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& /*faceCentre*/,
  TrackingData&
)
{}
template<class TrackingData>
inline bool mousse::minData::updateCell
(
  const polyMesh&,
  const label /*thisCellI*/,
  const label /*neighbourFaceI*/,
  const minData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (neighbourInfo.data_ < data_)
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
inline bool mousse::minData::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const label /*neighbourCellI*/,
  const minData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  // From cell to its faces.
  if (neighbourInfo.data_ < data_)
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
inline bool mousse::minData::updateFace
(
  const polyMesh&,
  const label /*thisFaceI*/,
  const minData& neighbourInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  // From face to face (e.g. coupled faces)
  if (neighbourInfo.data_ < data_)
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
inline bool mousse::minData::equal
(
  const minData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::minData::operator==
(
  const mousse::minData& rhs
) const
{
  return data() == rhs.data();
}
inline bool mousse::minData::operator!=
(
  const mousse::minData& rhs
) const
{
  return !(*this == rhs);
}
// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::minData& wDist
)
{
  return os << wDist.data_;
}
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::minData& wDist
)
{
  return is >> wDist.data_;
}
