// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
// Null constructor
inline mousse::refinementData::refinementData()
:
  refinementCount_(-1),
  count_(-1)
{}
// Construct from components
inline mousse::refinementData::refinementData
(
  const label refinementCount,
  const label count
)
:
  refinementCount_(refinementCount),
  count_(count)
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::refinementData::valid(TrackingData& td) const
{
  return count_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::refinementData::sameGeometry
(
  const polyMesh&,
  const refinementData&,
  const scalar,
  TrackingData& td
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::refinementData::leaveDomain
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
inline void mousse::refinementData::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData& td
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::refinementData::enterDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData& td
)
{}
// Update cell with neighbouring face information
template<class TrackingData>
inline bool mousse::refinementData::updateCell
(
  const polyMesh&,
  const label thisCellI,
  const label neighbourFaceI,
  const refinementData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td))
  {
    FatalErrorIn("refinementData::updateCell") << "problem"
      << abort(FatalError);
    return false;
  }
  // Check if more than 2:1 ratio. This is when I am not refined but neighbour
  // is and neighbour already had higher cell level.
  if
  (
    neighbourInfo.isRefined()
  && !isRefined()
  &&  neighbourInfo.refinementCount() > refinementCount()
  )
  {
    count_ = refinementCount();
    return true;
  }
  // Count from neighbour face by the time it reaches the current cell.
  label transportedFaceCount;
  if (neighbourInfo.isRefined())
  {
    // refined so passes through two cells.
    transportedFaceCount = max(0, neighbourInfo.count()-2);
  }
  else
  {
    // unrefined.
    transportedFaceCount = max(0, neighbourInfo.count()-1);
  }
  if (count_ >= transportedFaceCount)
  {
    return false;
  }
  else
  {
    count_ = transportedFaceCount;
    return true;
  }
}
// Update face with neighbouring cell information
template<class TrackingData>
inline bool mousse::refinementData::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const label neighbourCellI,
  const refinementData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  // From cell to its faces.
  if (!valid(td))
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
  if (count_ >= neighbourInfo.count())
  {
    return false;
  }
  else
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
}
// Update face with coupled face information
template<class TrackingData>
inline bool mousse::refinementData::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const refinementData& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  // From face to face (e.g. coupled faces)
  if (!valid(td))
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
  if (count_ >= neighbourInfo.count())
  {
    return false;
  }
  else
  {
    refinementCount_ = neighbourInfo.refinementCount();
    count_ = neighbourInfo.count();
    return true;
  }
}
template<class TrackingData>
inline bool mousse::refinementData::equal
(
  const refinementData& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::refinementData::operator==(const mousse::refinementData& rhs)
const
{
  return count() == rhs.count() && refinementCount() == rhs.refinementCount();
}
inline bool mousse::refinementData::operator!=(const mousse::refinementData& rhs)
const
{
  return !(*this == rhs);
}
