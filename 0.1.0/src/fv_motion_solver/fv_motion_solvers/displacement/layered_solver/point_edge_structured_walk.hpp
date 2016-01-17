// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointEdgeStructuredWalk
// Description
//   Determines length of string of edges walked to point.
// SourceFiles
//   point_edge_structured_walk.cpp
#ifndef point_edge_structured_walk_hpp_
#define point_edge_structured_walk_hpp_
#include "point.hpp"
#include "tensor.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"
namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class pointEdgeStructuredWalk
{
  // Private data
    //- Starting location
    point point0_;
    //- Previous point
    point previousPoint_;
    //- Sum of distance
    scalar dist_;
    //- Passive data
    vector data_;
  // Private Member Functions
    //- Evaluate distance to point.
    template<class TrackingData>
    inline bool update
    (
      const pointEdgeStructuredWalk& w2,
      const scalar tol,
      TrackingData& td
    );
public:
  // Constructors
    //- Construct null
    inline pointEdgeStructuredWalk();
    //- Construct from components
    inline pointEdgeStructuredWalk
    (
      const point&,
      const point&,
      const scalar,
      const vector&
    );
  // Member Functions
    // Access
      inline bool inZone() const;
      inline scalar dist() const;
      inline const vector& data() const;
    // Needed by meshWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const pointEdgeStructuredWalk&,
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
        const pointEdgeStructuredWalk& edgeInfo,
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
        const pointEdgeStructuredWalk& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  No information about current position whatsoever.
      template<class TrackingData>
      inline bool updatePoint
      (
        const pointEdgeStructuredWalk& newPointInfo,
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
        const pointEdgeStructuredWalk& pointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const pointEdgeStructuredWalk&, TrackingData&)
      const;
  // Member Operators
    //Note: Used to determine whether to call update.
    inline bool operator==(const pointEdgeStructuredWalk&) const;
    inline bool operator!=(const pointEdgeStructuredWalk&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const pointEdgeStructuredWalk&);
    friend Istream& operator>>(Istream&, pointEdgeStructuredWalk&);
};
//- Data associated with pointEdgeStructuredWalk type are contiguous
template<>
inline bool contiguous<pointEdgeStructuredWalk>()
{
  return true;
}
}  // namespace mousse

// Private Member Functions 
// Update this with w2.
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::update
(
  const pointEdgeStructuredWalk& w2,
  const scalar /*tol*/,
  TrackingData& td
)
{
  if (!valid(td))
  {
    // current not yet set. Walked from w2 to here (=point0)
    dist_ = w2.dist_ + mag(point0_-w2.previousPoint_);
    previousPoint_ = point0_;
    data_ = w2.data_;
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
// Null constructor
inline mousse::pointEdgeStructuredWalk::pointEdgeStructuredWalk()
:
  point0_{vector::max},
  previousPoint_{vector::max},
  dist_{0},
  data_{vector::zero}
{}
// Construct from origin, distance
inline mousse::pointEdgeStructuredWalk::pointEdgeStructuredWalk
(
  const point& point0,
  const point& previousPoint,
  const scalar dist,
  const vector& data
)
:
  point0_{point0},
  previousPoint_{previousPoint},
  dist_{dist},
  data_{data}
{}
// Member Functions 
inline bool mousse::pointEdgeStructuredWalk::inZone() const
{
  return point0_ != vector::max;
}
//inline const mousse::point& mousse::pointEdgeStructuredWalk::previousPoint() const
//{
//    return previousPoint_;
//}
inline mousse::scalar mousse::pointEdgeStructuredWalk::dist() const
{
  return dist_;
}
inline const mousse::vector& mousse::pointEdgeStructuredWalk::data() const
{
  return data_;
}
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::valid(TrackingData&) const
{
  return previousPoint_ != vector::max;
}
// Checks for cyclic points
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::sameGeometry
(
  const pointEdgeStructuredWalk& w2,
  const scalar tol,
  TrackingData&
) const
{
  scalar diff = mousse::mag(dist() - w2.dist());
  if (diff < SMALL)
  {
    return true;
  }
  else
  {
    if ((dist() > SMALL) && ((diff/dist()) < tol))
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
inline void mousse::pointEdgeStructuredWalk::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  previousPoint_ -= coord;
}
template<class TrackingData>
inline void mousse::pointEdgeStructuredWalk::transform
(
  const tensor& rotTensor,
  TrackingData&
)
{
  previousPoint_ = mousse::transform(rotTensor, previousPoint_);
}
// Update absolute geometric quantities. Note that distance (dist_)
// is not affected by leaving/entering domain.
template<class TrackingData>
inline void mousse::pointEdgeStructuredWalk::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& coord,
  TrackingData&
)
{
  // back to absolute form
  previousPoint_ += coord;
}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const label /*edgeI*/,
  const pointEdgeStructuredWalk& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(edgeInfo, tol, td);
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const pointEdgeStructuredWalk& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(newPointInfo, tol, td);
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updatePoint
(
  const pointEdgeStructuredWalk& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  return update(newPointInfo, tol, td);
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::updateEdge
(
  const polyMesh&,
  const label /*edgeI*/,
  const label /*pointI*/,
  const pointEdgeStructuredWalk& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (inZone())
  {
    return update(pointInfo, tol, td);
  }
  else
  {
    return false;
  }
}
template<class TrackingData>
inline bool mousse::pointEdgeStructuredWalk::equal
(
  const pointEdgeStructuredWalk& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::pointEdgeStructuredWalk::operator==
(
  const mousse::pointEdgeStructuredWalk& rhs
) const
{
  return previousPoint_ == rhs.previousPoint_;
}
inline bool mousse::pointEdgeStructuredWalk::operator!=
(
  const mousse::pointEdgeStructuredWalk& rhs
) const
{
  return !(*this == rhs);
}
#endif
