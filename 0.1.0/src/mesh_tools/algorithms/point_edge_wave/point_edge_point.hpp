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
//   point_edge_point_i.hpp
//   point_edge_point.cpp
#ifndef point_edge_point_hpp_
#define point_edge_point_hpp_
#include "point.hpp"
#include "label.hpp"
#include "scalar.hpp"
#include "tensor.hpp"
#include "ptraits.hpp"
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
#include "point_edge_point_i.hpp"
#endif
