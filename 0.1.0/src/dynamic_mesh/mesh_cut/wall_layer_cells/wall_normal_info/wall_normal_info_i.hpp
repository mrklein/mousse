// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Private Member Functions 
// Update this with w2 if not yet set.
template<class TrackingData>
inline bool mousse::wallNormalInfo::update
(
  const wallNormalInfo& w2,
  TrackingData& td
)
{
  if (!w2.valid(td))
  {
    FatalErrorIn
    (
      "wallNormalInfo::update(const wallNormalInfo&)"
    ) << "Problem: w2 is not valid" << abort(FatalError);
    return false;
  }
  else if (valid(td))
  {
    // our already set. Stop any transfer
    return false;
  }
  else
  {
    normal_ = w2.normal();
    return true;
  }
}
// Constructors 
// Null constructor
inline mousse::wallNormalInfo::wallNormalInfo()
:
  normal_(point::max)
{}
// Construct from normal
inline mousse::wallNormalInfo::wallNormalInfo(const vector& normal)
:
  normal_(normal)
{}
// Construct as copy
inline mousse::wallNormalInfo::wallNormalInfo(const wallNormalInfo& wpt)
:
  normal_(wpt.normal())
{}
// Member Functions 
inline const mousse::vector& mousse::wallNormalInfo::normal() const
{
  return normal_;
}
template<class TrackingData>
inline bool mousse::wallNormalInfo::valid(TrackingData& td) const
{
  return normal_ != point::max;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::wallNormalInfo::sameGeometry
(
  const polyMesh&,
  const wallNormalInfo& w2,
  const scalar tol,
  TrackingData& td
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::wallNormalInfo::leaveDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData& td
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::wallNormalInfo::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData& td
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::wallNormalInfo::enterDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData& td
)
{}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateCell
(
  const polyMesh&,
  const label thisCellI,
  const label neighbourFaceI,
  const wallNormalInfo& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const label neighbourCellI,
  const wallNormalInfo& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallNormalInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const wallNormalInfo& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(neighbourWallInfo, td);
}
template<class TrackingData>
inline bool mousse::wallNormalInfo::equal
(
  const wallNormalInfo& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::wallNormalInfo::operator==(const mousse::wallNormalInfo& rhs)
const
{
  return normal() == rhs.normal();
}
inline bool mousse::wallNormalInfo::operator!=(const mousse::wallNormalInfo& rhs)
const
{
  return !(*this == rhs);
}
