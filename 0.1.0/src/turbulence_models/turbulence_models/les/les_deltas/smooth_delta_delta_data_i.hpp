// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::update
(
  const smoothDelta::deltaData& w2,
  const scalar scale,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td) || (delta_ < VSMALL))
  {
    // My delta not set. Take over neighbour.
    delta_ = w2.delta()/scale;
    // Something changed. Let caller know.
    return true;
  }
  else if (w2.delta() > (1 + tol)*scale*delta_)
  {
    // Neighbour is too big for me. Up my delta.
    delta_ = w2.delta()/scale;
    // Something changed. Let caller know.
    return true;
  }
  else
  {
    // Neighbour is not too big for me or change is too small
    // Nothing changed.
    return false;
  }
}
// Constructors 
inline mousse::LESModels::smoothDelta::deltaData::deltaData()
:
  delta_(-GREAT)
{}
inline mousse::LESModels::smoothDelta::deltaData::deltaData(const scalar delta)
:
  delta_(delta)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::valid
(
  TrackingData& td
) const
{
  return delta_ > -SMALL;
}
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::sameGeometry
(
  const polyMesh&,
  const deltaData&,
  const scalar,
  TrackingData& td
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::LESModels::smoothDelta::deltaData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData& td
)
{}
template<class TrackingData>
inline void mousse::LESModels::smoothDelta::deltaData::transform
(
  const polyMesh&,
  const tensor&,
  TrackingData& td
)
{}
template<class TrackingData>
inline void mousse::LESModels::smoothDelta::deltaData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point&,
  TrackingData& td
)
{}
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::updateCell
(
  const polyMesh&,
  const label,
  const label,
  const deltaData& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  // Take over info from face if more than deltaRatio larger.
  return update(neighbourWallInfo, td, tol, td);
}
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::updateFace
(
  const polyMesh&,
  const label,
  const label,
  const deltaData& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from cell without any scaling (scale = 1.0)
  return update(neighbourWallInfo, 1.0, tol, td);
}
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::updateFace
(
  const polyMesh&,
  const label,
  const deltaData& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  // Take over information from coupled face without any scaling (scale = 1.0)
  return update(neighbourWallInfo, 1.0, tol, td);
}
template<class TrackingData>
inline bool mousse::LESModels::smoothDelta::deltaData::equal
(
  const deltaData& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::LESModels::smoothDelta::deltaData::operator==
(
  const deltaData& rhs
) const
{
  return delta_ == rhs.delta();
}
inline bool mousse::LESModels::smoothDelta::deltaData::operator!=
(
  const deltaData& rhs
) const
{
  return !(*this == rhs);
}
