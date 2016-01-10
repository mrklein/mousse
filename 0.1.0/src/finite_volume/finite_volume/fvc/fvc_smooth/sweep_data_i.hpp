// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform.hpp"
// Private Member Functions 
template<class TrackingData>
inline bool mousse::sweepData::update
(
  const sweepData& svf,
  const point& position,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!valid(td))
  {
    operator=(svf);
    return true;
  }
  scalar myDist2 = magSqr(position - origin());
  if (myDist2 < SMALL)
  {
    if (svf.value() > value())
    {
      operator=(svf);
      return true;
    }
    else
    {
      return false;
    }
  }
  scalar dist2 = magSqr(position - svf.origin());
  if (dist2 < myDist2)
  {
    operator=(svf);
    return true;
  }
  return false;
}
// Constructors 
inline mousse::sweepData::sweepData()
:
  value_(-GREAT),
  origin_(vector::max)
{}
inline mousse::sweepData::sweepData(const scalar value, const point& origin)
:
  value_(value),
  origin_(origin)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::sweepData::valid(TrackingData&) const
{
  return value_ > -SMALL;
}
template<class TrackingData>
inline bool mousse::sweepData::sameGeometry
(
  const polyMesh&,
  const sweepData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::sweepData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point& faceCentre,
  TrackingData&
)
{
  origin_ -= faceCentre;
}
template<class TrackingData>
inline void mousse::sweepData::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
template<class TrackingData>
inline void mousse::sweepData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label,
  const point& faceCentre,
  TrackingData&
)
{
  // back to absolute form
  origin_ += faceCentre;
}
template<class TrackingData>
inline bool mousse::sweepData::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label,
  const sweepData& svf,
  const scalar tol,
  TrackingData& td
)
{
  return update(svf, mesh.cellCentres()[thisCellI], tol, td);
}
template<class TrackingData>
inline bool mousse::sweepData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label,
  const sweepData& svf,
  const scalar tol,
  TrackingData& td
)
{
  return update(svf, mesh.faceCentres()[thisFaceI], tol, td);
}
// Update this (face) with coupled face information.
template<class TrackingData>
inline bool mousse::sweepData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const sweepData& svf,
  const scalar tol,
  TrackingData& td
)
{
  return update(svf, mesh.faceCentres()[thisFaceI], tol, td);
}
template<class TrackingData>
inline bool mousse::sweepData::equal
(
  const sweepData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline void mousse::sweepData::operator=
(
  const scalar value
)
{
  value_ = value;
}
inline bool mousse::sweepData::operator==
(
  const sweepData& rhs
) const
{
  return origin() == rhs.origin();
}
inline bool mousse::sweepData::operator!=
(
  const sweepData& rhs
) const
{
  return !(*this == rhs);
}
