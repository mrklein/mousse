#ifndef MESH_TOOLS_CELL_DIST_WALL_POINT_WALL_POINT_HPP_
#define MESH_TOOLS_CELL_DIST_WALL_POINT_WALL_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPoint
// Description
//   Holds information regarding nearest wall point. Used in wall distance
//   calculation.
// SourceFiles
//   wall_point.cpp
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"

namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class wallPoint;
// Forward declaration of friend functions and operators
Ostream& operator<<(Ostream&, const wallPoint&);
Istream& operator>>(Istream&, wallPoint&);
class wallPoint
{
  // Private data
    //- Position of nearest wall center
    point origin_;
    //- Normal distance (squared) from cellcenter to origin
    scalar distSqr_;
  // Private Member Functions
    //- Evaluate distance to point. Update distSqr, origin from whomever
    //  is nearer pt. Return true if w2 is closer to point,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const wallPoint& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline wallPoint();
    //- Construct from origin, distance
    inline wallPoint(const point& origin, const scalar distSqr);
    //- Construct as copy
    inline wallPoint(const wallPoint&);
  // Member Functions
    // Access
      inline const point& origin() const;
      inline point& origin();
      inline scalar distSqr() const;
      inline scalar& distSqr();
    // Needed by FaceCellWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const polyMesh&,
        const wallPoint&,
        const scalar,
        TrackingData& td
      ) const;
      //- Convert any absolute coordinates into relative to (patch)face
      //  centre
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Reverse of leaveDomain
      template<class TrackingData>
      inline void enterDomain
      (
        const polyMesh&,
        const polyPatch&,
        const label patchFaceI,
        const point& faceCentre,
        TrackingData& td
      );
      //- Apply rotation matrix to any coordinates
      template<class TrackingData>
      inline void transform
      (
        const polyMesh&,
        const tensor&,
        TrackingData& td
      );
      //- Influence of neighbouring face.
      template<class TrackingData>
      inline bool updateCell
      (
        const polyMesh&,
        const label thisCellI,
        const label neighbourFaceI,
        const wallPoint& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of neighbouring cell.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const label neighbourCellI,
        const wallPoint& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same face.
      template<class TrackingData>
      inline bool updateFace
      (
        const polyMesh&,
        const label thisFaceI,
        const wallPoint& neighbourInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const wallPoint&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const wallPoint&) const;
    inline bool operator!=(const wallPoint&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const wallPoint&);
    friend Istream& operator>>(Istream&, wallPoint&);
};
//- Data associated with wallPoint type are contiguous
template<>
inline bool contiguous<wallPoint>()
{
  return true;
}
}  // namespace mousse

// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallPoint::update
(
  const point& pt,
  const wallPoint& w2,
  const scalar tol,
  TrackingData& td
)
{
  //Already done in calling algorithm
  //if (w2.origin() == origin_)
  //{
  //    // Shortcut. Same input so same distance.
  //    return false;
  //}
  scalar dist2 = magSqr(pt - w2.origin());
  if (!valid(td))
  {
    // current not yet set so use any value
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - dist2;
  if (diff < 0)
  {
    // already nearer to pt
    return false;
  }
  if ((diff < SMALL) || ((distSqr_ > SMALL) && (diff/distSqr_ < tol)))
  {
    // don't propagate small changes
    return false;
  }
  else
  {
    // update with new values
    distSqr_ = dist2;
    origin_ = w2.origin();
    return true;
  }
}
// Constructors 
inline mousse::wallPoint::wallPoint()
:
  origin_{point::max},
  distSqr_{-GREAT}
{}
inline mousse::wallPoint::wallPoint(const point& origin, const scalar distSqr)
:
  origin_{origin},
  distSqr_{distSqr}
{}
inline mousse::wallPoint::wallPoint(const wallPoint& wpt)
:
  origin_{wpt.origin()},
  distSqr_{wpt.distSqr()}
{}
// Member Functions 
inline const mousse::point& mousse::wallPoint::origin() const
{
  return origin_;
}
inline mousse::point& mousse::wallPoint::origin()
{
  return origin_;
}
inline mousse::scalar mousse::wallPoint::distSqr() const
{
  return distSqr_;
}
inline mousse::scalar& mousse::wallPoint::distSqr()
{
  return distSqr_;
}
template<class TrackingData>
inline bool mousse::wallPoint::valid(TrackingData&) const
{
  return distSqr_ > -SMALL;
}
// Checks for cyclic faces
template<class TrackingData>
inline bool mousse::wallPoint::sameGeometry
(
  const polyMesh&,
  const wallPoint& w2,
  const scalar tol,
  TrackingData&
) const
{
  scalar diff = mag(distSqr() - w2.distSqr());
  if (diff < SMALL)
  {
    return true;
  }
  else
  {
    if ((distSqr() > SMALL) && ((diff/distSqr()) < tol))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}
template<class TrackingData>
inline void mousse::wallPoint::leaveDomain
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
inline void mousse::wallPoint::transform
(
  const polyMesh&,
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
// Update absolute geometric quantities. Note that distance (distSqr_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void mousse::wallPoint::enterDomain
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
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallPoint::updateCell
(
  const polyMesh& mesh,
  const label thisCellI,
  const label /*neighbourFaceI*/,
  const wallPoint& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return
    update
    (
      mesh.cellCentres()[thisCellI],
      neighbourWallInfo,
      tol,
      td
    );
  }
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallPoint::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const label /*neighbourCellI*/,
  const wallPoint& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return
    update
    (
      mesh.faceCentres()[thisFaceI],
      neighbourWallInfo,
      tol,
      td
    );
}
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::wallPoint::updateFace
(
  const polyMesh& mesh,
  const label thisFaceI,
  const wallPoint& neighbourWallInfo,
  const scalar tol,
  TrackingData& td
)
{
  return
    update
    (
      mesh.faceCentres()[thisFaceI],
      neighbourWallInfo,
      tol,
      td
    );
}
template<class TrackingData>
inline bool mousse::wallPoint::equal
(
  const wallPoint& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::wallPoint::operator==(const mousse::wallPoint& rhs) const
{
  return origin() == rhs.origin();
}
inline bool mousse::wallPoint::operator!=(const mousse::wallPoint& rhs) const
{
  return !(*this == rhs);
}
#endif
