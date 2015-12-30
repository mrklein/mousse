// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointEdgeStructuredWalk
// Description
//   Determines length of string of edges walked to point.
// SourceFiles
//   point_edge_structured_walk_i.hpp
//   point_edge_structured_walk.cpp
#ifndef point_edge_structured_walk_hpp_
#define point_edge_structured_walk_hpp_
#include "point.hpp"
#include "tensor.hpp"
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
#include "point_edge_structured_walk_i.hpp"
#endif
