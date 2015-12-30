// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class TrackingData>
inline bool mousse::smoothData::update
(
  const smoothData& svf,
  const scalar scale,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td) || (value_ < VSMALL))
  {
    // My value not set - take over neighbour
    value_ = svf.value()/scale;
    // Something changed - let caller know
    return true;
  }
  else if (svf.value() > (1 + tol)*scale*value_)
  {
    // Neighbour is too big for me - Up my value
    value_ = svf.value()/scale;
    // Something changed - let caller know
    return true;
  }
  else
  {
    // Neighbour is not too big for me or change is too small
    // Nothing changed
    return false;
  }
}
// Constructors 
inline mousse::smoothData::smoothData()
:
  value_(-GREAT)
{}
inline mousse::smoothData::smoothData(const scalar value)
:
  value_(value)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::smoothData::valid(TrackingData& td) const
{
  return value_ > -SMALL;
}
template<class TrackingData>
inline bool mousse::smoothData::sameGeometry
(
  const polyMesh&,
  const smoothData&,
  const scalar,
  TrackingData& td
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::smoothData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData& td
)
{}
template<class TrackingData>
inline void mousse::smoothData::transform
(
  const polyMesh&,
  const tensor&,
  TrackingData& td
)
{}
template<class TrackingData>
inline void mousse::smoothData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData& td
)
{}
template<class TrackingData>
inline bool mousse::smoothData::updateCell
(
  const polyMesh&,
  const label,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over info from face if more than deltaRatio larger
  return update(svf, td.maxRatio, tol, td);
}
template<class TrackingData>
inline bool mousse::smoothData::updateFace
(
  const polyMesh&,
  const label,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from cell without any scaling (scale = 1.0)
  return update(svf, 1.0, tol, td);
}
// Update this (face) with coupled face information.
template<class TrackingData>
inline bool mousse::smoothData::updateFace
(
  const polyMesh&,
  const label,
  const smoothData& svf,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from coupled face without any scaling (scale = 1.0)
  return update(svf, 1.0, tol, td);
}
template<class TrackingData>
inline bool mousse::smoothData::equal
(
  const smoothData& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline void mousse::smoothData::operator=
(
  const scalar value
)
{
  value_ = value;
}
inline bool mousse::smoothData::operator==
(
  const smoothData& rhs
) const
{
  return value_ == rhs.value();
}
inline bool mousse::smoothData::operator!=
(
  const smoothData& rhs
) const
{
  return !(*this == rhs);
}
