// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform.hpp"
#include "poly_mesh.hpp"
// Private Member Functions 
// Returns the wanted level
inline mousse::label mousse::refinementDistanceData::wantedLevel(const point& pt)
const
{
  const scalar distSqr = magSqr(pt-origin_);
  // Get the size at the origin level
  scalar levelSize = level0Size_/(1<<originLevel_);
  scalar r = 0;
  for (label level = originLevel_; level >= 0; --level)
  {
    // Current range
    r += levelSize;
    // Check if our distance is within influence sphere
    if (sqr(r) > distSqr)
    {
      return level;
    }
    // Lower level will have double the size
    levelSize *= 2;
  }
  return 0;
}
template<class TrackingData>
inline bool mousse::refinementDistanceData::update
(
  const point& pos,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td))
  {
    if (!neighbourInfo.valid(td))
    {
      FATAL_ERROR_IN("refinementDistanceData::update(..)")
        << "problem" << abort(FatalError);
    }
    operator=(neighbourInfo);
    return true;
  }
  // Determine wanted level at current position.
  label cellLevel = wantedLevel(pos);
  // Determine wanted level coming through the neighbour
  label nbrLevel = neighbourInfo.wantedLevel(pos);
  if (nbrLevel > cellLevel)
  {
    operator=(neighbourInfo);
    return true;
  }
  else if (nbrLevel == cellLevel)
  {
    scalar myDistSqr = magSqr(pos-origin_);
    scalar nbrDistSqr = magSqr(pos - neighbourInfo.origin());
    scalar diff = myDistSqr - nbrDistSqr;
    if (diff < 0)
    {
      // already nearest
      return false;
    }
    if ((diff < SMALL) || ((myDistSqr > SMALL) && (diff/myDistSqr < tol)))
    {
      // don't propagate small changes
      return false;
    }
    else
    {
      // update with new values
      operator=(neighbourInfo);
      return true;
    }
  }
  else
  {
    return false;
  }
}
// Constructors 
// Null constructor
inline mousse::refinementDistanceData::refinementDistanceData()
:
  level0Size_(-1)
{}
// Construct from components
inline mousse::refinementDistanceData::refinementDistanceData
(
  const scalar level0Size,
  const point& origin,
  const label originLevel
)
:
  level0Size_(level0Size),
  origin_(origin),
  originLevel_(originLevel)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::refinementDistanceData::valid(TrackingData&) const
{
  return level0Size_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::refinementDistanceData::sameGeometry
(
  const polyMesh&,
  const refinementDistanceData&,
  const scalar,
  TrackingData&
) const
{
  return true;
}
template<class TrackingData>
inline void mousse::refinementDistanceData::leaveDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& faceCentre,
  TrackingData&
)
{
  origin_ -= faceCentre;
}
template<class TrackingData>
inline void mousse::refinementDistanceData::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
// Update absolute geometric quantities.
template<class TrackingData>
inline void mousse::refinementDistanceData::enterDomain
(
  const polyMesh&,
  const polyPatch&,
  const label /*patchFaceI*/,
  const point& faceCentre,
  TrackingData&
)
{
  // back to absolute form
  origin_ += faceCentre;
}
// Update cell with neighbouring face information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label /*neighbourFaceI*/,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.cellCentres()[thisCellI];
  return update(pos, neighbourInfo, tol, td);
}
// Update face with neighbouring cell information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label /*neighbourCellI*/,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.faceCentres()[thisFaceI];
  return update(pos, neighbourInfo, tol, td);
}
// Update face with coupled face information
template<class TrackingData>
inline bool mousse::refinementDistanceData::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const refinementDistanceData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  const point& pos = mesh.faceCentres()[thisFaceI];
  return update(pos, neighbourInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::refinementDistanceData::equal
(
  const refinementDistanceData& rhs,
  TrackingData& td
) const
{
  if (!valid(td))
  {
    if (!rhs.valid(td))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return operator==(rhs);
  }
}
// Member Operators 
inline bool mousse::refinementDistanceData::operator==
(
  const mousse::refinementDistanceData& rhs
)
const
{
  return
    level0Size_ == rhs.level0Size_
  && origin_ == rhs.origin_
  && originLevel_ == rhs.originLevel_;
}
inline bool mousse::refinementDistanceData::operator!=
(
  const mousse::refinementDistanceData& rhs
)
const
{
  return !(*this == rhs);
}
