// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointEdgeCollapse
// Description
//   Determines length of string of edges walked to point.
// SourceFiles
//   point_edge_collapse_i.hpp
//   point_edge_collapse.cpp
#ifndef point_edge_collapse_hpp_
#define point_edge_collapse_hpp_
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
// Forward declaration of classes
class polyPatch;
class polyMesh;
class pointEdgeCollapse
{
  // Private data
    //- Collapse location
    point collapsePoint_;
    //- Collapse string index
    label collapseIndex_;
    //- Priority of the collapse
    label collapsePriority_;
  // Private Member Functions
    //- Evaluate distance to point.
    template<class TrackingData>
    inline bool update
    (
      const pointEdgeCollapse& w2,
      const scalar tol,
      TrackingData& td
    );
    //- Check for same coordinate
    inline bool samePoint(const point& pt) const;
public:
  // Constructors
    //- Construct null
    inline pointEdgeCollapse();
    //- Construct from components
    inline pointEdgeCollapse
    (
      const point& collapsePoint,
      const label collapseIndex,
      const label collapsePriority
    );
  // Member Functions
    // Access
      inline const point& collapsePoint() const;
      inline label collapseIndex() const;
      inline label collapsePriority() const;
    // Needed by meshWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
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
        const pointEdgeCollapse& edgeInfo,
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
        const pointEdgeCollapse& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  No information about current position whatsoever.
      template<class TrackingData>
      inline bool updatePoint
      (
        const pointEdgeCollapse& newPointInfo,
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
        const pointEdgeCollapse& pointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const pointEdgeCollapse&, TrackingData&)
      const;
  // Member Operators
    //Note: Used to determine whether to call update.
    inline bool operator==(const pointEdgeCollapse&) const;
    inline bool operator!=(const pointEdgeCollapse&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const pointEdgeCollapse&);
    friend Istream& operator>>(Istream&, pointEdgeCollapse&);
};
//- Data associated with pointEdgeCollapse type are contiguous
template<>
inline bool contiguous<pointEdgeCollapse>()
{
  return true;
}
}  // namespace mousse
#include "point_edge_collapse_i.hpp"
#endif
