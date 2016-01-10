// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "transform.hpp"
// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::update
(
  const patchEdgeFaceRegion& w2,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!w2.valid(td))
  {
    FATAL_ERROR_IN("patchEdgeFaceRegion::update(..)")
      << "problem." << abort(FatalError);
  }
  if (w2.region_ == -2 || region_ == -2)
  {
    // Blocked edge/face
    return false;
  }
  if (!valid(td))
  {
    // current not yet set so use any value
    operator=(w2);
    return true;
  }
  else
  {
    if (w2.region_ < region_)
    {
      operator=(w2);
      return true;
    }
    else
    {
      return false;
    }
  }
}
// Constructors 
// Null constructor
inline mousse::patchEdgeFaceRegion::patchEdgeFaceRegion()
:
  region_(-1)
{}
// Construct from origin, distance
inline mousse::patchEdgeFaceRegion::patchEdgeFaceRegion
(
  const label region
)
:
  region_(region)
{}
// Member Functions 
inline mousse::label mousse::patchEdgeFaceRegion::region() const
{
  return region_;
}
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::valid(TrackingData&) const
{
  return region_ != -1;
}
template<class TrackingData>
inline void mousse::patchEdgeFaceRegion::transform
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const tensor& /*rotTensor*/,
  const scalar /*tol*/,
  TrackingData&
)
{}
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const label /*edgeI*/,
  const label /*faceI*/,
  const patchEdgeFaceRegion& faceInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(faceInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateEdge
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const patchEdgeFaceRegion& edgeInfo,
  const bool /*sameOrientation*/,
  const scalar tol,
  TrackingData& td
)
{
  return update(edgeInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::updateFace
(
  const polyMesh&,
  const indirectPrimitivePatch&,
  const label /*faceI*/,
  const label /*edgeI*/,
  const patchEdgeFaceRegion& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(edgeInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::patchEdgeFaceRegion::equal
(
  const patchEdgeFaceRegion& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::patchEdgeFaceRegion::operator==
(
  const mousse::patchEdgeFaceRegion& rhs
) const
{
  return region() == rhs.region();
}
inline bool mousse::patchEdgeFaceRegion::operator!=
(
  const mousse::patchEdgeFaceRegion& rhs
) const
{
  return !(*this == rhs);
}
