// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class Type>
template<class TrackingData>
inline bool wallPointData<Type>::update
(
  const point& pt,
  const wallPointData<Type>& w2,
  const scalar tol,
  TrackingData& td
)
{
  scalar dist2 = magSqr(pt - w2.origin());
  if (valid(td))
  {
    scalar diff = distSqr() - dist2;
    if (diff < 0)
    {
      // already nearer to pt
      return false;
    }
    if ((diff < SMALL) || ((distSqr() > SMALL) && (diff/distSqr() < tol)))
    {
      // don't propagate small changes
      return false;
    }
  }
  // Either *this is not yet valid or w2 is closer
  {
    // current not yet set so use any value
    distSqr() = dist2;
    origin() = w2.origin();
    data_ = w2.data();
    return true;
  }
}
// Constructors 
// Null constructor
template<class Type>
inline wallPointData<Type>::wallPointData()
:
  wallPoint(),
  data_()
{}
// Construct from components
template<class Type>
inline wallPointData<Type>::wallPointData
(
  const point& origin,
  const Type& data,
  const scalar distSqr
)
:
  wallPoint(origin, distSqr),
  data_(data)
{}
// Member Functions 
template<class Type>
inline const Type& wallPointData<Type>::data() const
{
  return data_;
}
template<class Type>
inline Type& wallPointData<Type>::data()
{
  return data_;
}
// Update this with w2 if w2 nearer to pt.
template<class Type>
template<class TrackingData>
inline bool wallPointData<Type>::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label,
  const wallPointData<Type>& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& cellCentres = mesh.primitiveMesh::cellCentres();
  return update
  (
    cellCentres[thisCellI],
    neighbourWallInfo,
    tol,
    td
  );
}
// Update this with w2 if w2 nearer to pt.
template<class Type>
template<class TrackingData>
inline bool wallPointData<Type>::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label,
  const wallPointData<Type>& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& faceCentres = mesh.faceCentres();
  return update
  (
    faceCentres[thisFaceI],
    neighbourWallInfo,
    tol,
    td
  );
}
// Update this with w2 if w2 nearer to pt.
template<class Type>
template<class TrackingData>
inline bool wallPointData<Type>::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const wallPointData<Type>& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  const vectorField& faceCentres = mesh.faceCentres();
  return update
  (
    faceCentres[thisFaceI],
    neighbourWallInfo,
    tol,
    td
  );
}
}  // namespace mousse
