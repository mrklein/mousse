#ifndef MESH_TOOLS_ALGORITHMS_POINT_EDGE_WAVE_POINT_EDGE_POINT_HPP_
#define MESH_TOOLS_ALGORITHMS_POINT_EDGE_WAVE_POINT_EDGE_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointEdgePoint
// Description
//   Holds information regarding nearest wall point. Used in PointEdgeWave.
//   (so not standard FaceCellWave)
//   To be used in wall distance calculation.
// SourceFiles
//   point_edge_point.cpp
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "ptraits.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"

namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class pointEdgePoint
{
  // Private data
    //- Position of nearest wall center
    point origin_;
    //- Normal distance (squared) from point to origin
    scalar distSqr_;
  // Private Member Functions
    //- Evaluate distance to point. Update distSqr, origin from whomever
    //  is nearer pt. Return true if w2 is closer to point,
    //  false otherwise.
    template<class TrackingData>
    inline bool update
    (
      const point&,
      const pointEdgePoint& w2,
      const scalar tol,
      TrackingData& td
    );
    //- Combine current with w2. Update distSqr, origin if w2 has smaller
    //  quantities and returns true.
    template<class TrackingData>
    inline bool update
    (
      const pointEdgePoint& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline pointEdgePoint();
    //- Construct from origin, distance
    inline pointEdgePoint(const point&, const scalar);
    //- Construct as copy
    inline pointEdgePoint(const pointEdgePoint&);
  // Member Functions
    // Access
      inline const point& origin() const;
      inline scalar distSqr() const;
    // Needed by PointEdgeWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const pointEdgePoint&,
        const scalar tol,
        TrackingData& td
      ) const;
      //- Convert origin to relative vector to leaving point
      //  (= point coordinate)
      template<class TrackingData>
      inline void leaveDomain
      (
        const polyPatch& patch,
        const label patchPointI,
        const point& pos,
        TrackingData& td
      );
      //- Convert relative origin to absolute by adding entering point
      template<class TrackingData>
      inline void enterDomain
      (
        const polyPatch& patch,
        const label patchPointI,
        const point& pos,
        TrackingData& td
      );
      //- Apply rotation matrix to origin
      template<class TrackingData>
      inline void transform
      (
        const tensor& rotTensor,
        TrackingData& td
      );
      //- Influence of edge on point
      template<class TrackingData>
      inline bool updatePoint
      (
        const polyMesh& mesh,
        const label pointI,
        const label edgeI,
        const pointEdgePoint& edgeInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  Merge new and old info.
      template<class TrackingData>
      inline bool updatePoint
      (
        const polyMesh& mesh,
        const label pointI,
        const pointEdgePoint& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  No information about current position whatsoever.
      template<class TrackingData>
      inline bool updatePoint
      (
        const pointEdgePoint& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of point on edge.
      template<class TrackingData>
      inline bool updateEdge
      (
        const polyMesh& mesh,
        const label edgeI,
        const label pointI,
        const pointEdgePoint& pointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const pointEdgePoint&, TrackingData& td) const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const pointEdgePoint&) const;
    inline bool operator!=(const pointEdgePoint&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const pointEdgePoint&);
    friend Istream& operator>>(Istream&, pointEdgePoint&);
};
//- Data associated with pointEdgePoint type are contiguous
template<>
inline bool contiguous<pointEdgePoint>()
{
  return true;
}
}  // namespace mousse

// Private Member Functions 
// Update this with w2 if w2 nearer to pt.
template<class TrackingData>
inline bool mousse::pointEdgePoint::update
(
  const point& pt,
  const pointEdgePoint& w2,
  const scalar tol,
  TrackingData& td
)
{
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
// Update this with w2 (information on same point)
template<class TrackingData>
inline bool mousse::pointEdgePoint::update
(
  const pointEdgePoint& w2,
  const scalar tol,
  TrackingData& td
)
{
  if (!valid(td))
  {
    // current not yet set so use any value
    distSqr_ = w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
  scalar diff = distSqr_ - w2.distSqr();
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
    distSqr_ =  w2.distSqr();
    origin_ = w2.origin();
    return true;
  }
}
// Constructors 
// Null constructor
inline mousse::pointEdgePoint::pointEdgePoint()
:
  origin_{point::max},
  distSqr_{GREAT}
{}
// Construct from origin, distance
inline mousse::pointEdgePoint::pointEdgePoint
(
  const point& origin,
  const scalar distSqr
)
:
  origin_{origin},
  distSqr_{distSqr}
{}
// Construct as copy
inline mousse::pointEdgePoint::pointEdgePoint(const pointEdgePoint& wpt)
:
  origin_{wpt.origin()},
  distSqr_{wpt.distSqr()}
{}
// Member Functions 
inline const mousse::point& mousse::pointEdgePoint::origin() const
{
  return origin_;
}
inline mousse::scalar mousse::pointEdgePoint::distSqr() const
{
  return distSqr_;
}
template<class TrackingData>
inline bool mousse::pointEdgePoint::valid(TrackingData&) const
{
  return origin_ != point::max;
}
// Checks for cyclic points
template<class TrackingData>
inline bool mousse::pointEdgePoint::sameGeometry
(
  const pointEdgePoint& w2,
  const scalar tol,
  TrackingData&
) const
{
  scalar diff = mousse::mag(distSqr() - w2.distSqr());
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
inline void mousse::pointEdgePoint::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  origin_ -= coord;
}
template<class TrackingData>
inline void mousse::pointEdgePoint::transform
(
  const tensor& rotTensor,
  TrackingData&
)
{
  origin_ = mousse::transform(rotTensor, origin_);
}
// Update absolute geometric quantities. Note that distance (distSqr_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void mousse::pointEdgePoint::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  // back to absolute form
  origin_ += coord;
}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointEdgePoint::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const label /*edgeI*/,
  const pointEdgePoint& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(mesh.points()[pointI], edgeInfo, tol, td);
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointEdgePoint::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const pointEdgePoint& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(mesh.points()[pointI], newPointInfo, tol, td);
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointEdgePoint::updatePoint
(
  const pointEdgePoint& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(newPointInfo, tol, td);
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointEdgePoint::updateEdge
(
  const polyMesh& mesh,
  const label edgeI,
  const label /*pointI*/,
  const pointEdgePoint& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  const edge& e = mesh.edges()[edgeI];
  return update(e.centre(mesh.points()), pointInfo, tol, td);
}
template<class TrackingData>
inline bool mousse::pointEdgePoint::equal
(
  const pointEdgePoint& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::pointEdgePoint::operator==(const mousse::pointEdgePoint& rhs)
const
{
  return (origin() == rhs.origin()) && (distSqr() == rhs.distSqr());
}
inline bool mousse::pointEdgePoint::operator!=(const mousse::pointEdgePoint& rhs)
const
{
  return !(*this == rhs);
}
#endif
