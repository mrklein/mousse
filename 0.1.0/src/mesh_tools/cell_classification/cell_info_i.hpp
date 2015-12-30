// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_classification.hpp"
#include "poly_mesh.hpp"
// Private Member Functions 
// Update this with w2 information
template<class TrackingData>
inline bool mousse::cellInfo::update
(
  const cellInfo& w2,
  const label thisFaceI,
  const label thisCellI,
  const label neighbourFaceI,
  const label neighbourCellI,
  TrackingData& td
)
{
  if
  (
    (w2.type() == cellClassification::NOTSET)
  || (w2.type() == cellClassification::CUT)
  )
  {
    FatalErrorIn("cellInfo::update(const cellInfo&)")
      << "Problem: trying to propagate NOTSET or CUT type:" << w2.type()
      << " into cell/face with type:" << type() << endl
      << "thisFaceI:" << thisFaceI
      << "  thisCellI:" << thisCellI
      << "  neighbourFaceI:" << neighbourFaceI
      << "  neighbourCellI:" << neighbourCellI
      << abort(FatalError);
    return false;
  }
  if (type() == cellClassification::NOTSET)
  {
    type_ = w2.type();
    return true;
  }
  if (type() == cellClassification::CUT)
  {
    // Reached boundary. Stop.
    return false;
  }
  if (type() == w2.type())
  {
    // Should never happen; already checked in meshWave
    return false;
  }
  // Two conflicting types
  FatalErrorIn("cellInfo::update(const cellInfo&)")
    << "Problem: trying to propagate conflicting types:" << w2.type()
    << " into cell/face with type:" << type() << endl
    << "thisFaceI:" << thisFaceI
    << "  thisCellI:" << thisCellI
    << "  neighbourFaceI:" << neighbourFaceI
    << "  neighbourCellI:" << neighbourCellI
    << abort(FatalError);
  return false;
}
// Constructors 
// Null constructor
inline mousse::cellInfo::cellInfo()
:
  type_(cellClassification::NOTSET)
{}
// Construct from components
inline mousse::cellInfo::cellInfo(const label type)
:
  type_(type)
{}
// Construct as copy
inline mousse::cellInfo::cellInfo(const cellInfo& w2)
:
  type_(w2.type())
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::cellInfo::valid(TrackingData& td) const
{
  return type_ != cellClassification::NOTSET;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::cellInfo::sameGeometry
(
  const polyMesh&,
  const cellInfo& w2,
  const scalar tol,
  TrackingData& td
)
const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::cellInfo::leaveDomain
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
inline void mousse::cellInfo::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData& td
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::cellInfo::enterDomain
(
  const polyMesh&,
  const polyPatch& patch,
  const label patchFaceI,
  const point& faceCentre,
  TrackingData& td
)
{}
// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateCell
(
  const polyMesh&,
  const label thisCellI,
  const label neighbourFaceI,
  const cellInfo& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    -1,
    thisCellI,
    neighbourFaceI,
    -1,
    td
  );
}
// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const label neighbourCellI,
  const cellInfo& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    thisFaceI,
    -1,
    -1,
    neighbourCellI,
    td
  );
}
// Update this with neighbour information
template<class TrackingData>
inline bool mousse::cellInfo::updateFace
(
  const polyMesh&,
  const label thisFaceI,
  const cellInfo& neighbourInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update
  (
    neighbourInfo,
    thisFaceI,
    -1,
    -1,
    -1,
    td
  );
}
template<class TrackingData>
inline bool mousse::cellInfo::equal
(
  const cellInfo& rhs,
  TrackingData& td
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::cellInfo::operator==(const mousse::cellInfo& rhs) const
{
  return type() == rhs.type();
}
inline bool mousse::cellInfo::operator!=(const mousse::cellInfo& rhs) const
{
  return !(*this == rhs);
}
