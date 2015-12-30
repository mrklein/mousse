// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointData
// Description
//   Variant of pointEdgePoint with some transported additional data.
//   WIP - should be templated on data like wallDistData.
//   Passive vector v_ is not a coordinate (so no enterDomain/leaveDomain
//   transformation needed)
// SourceFiles
//   point_data_i.hpp
//   point_data.cpp
#ifndef point_data_hpp_
#define point_data_hpp_
#include "point_edge_point.hpp"
namespace mousse
{
class pointData
:
  public pointEdgePoint
{
  // Private data
    //- Additional information.
    scalar s_;
    //- Additional information.
    vector v_;
public:
  // Constructors
    //- Construct null
    inline pointData();
    //- Construct from origin, distance
    inline pointData
    (
      const point& origin,
      const scalar distSqr,
      const scalar s,
      const vector& v
    );
    //- Construct as copy
    inline pointData(const pointData&);
  // Member Functions
    // Access
      inline scalar s() const;
      inline const vector& v() const;
    // Needed by meshWave
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
        const pointData& edgeInfo,
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
        const pointData& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  No information about current position whatsoever.
      template<class TrackingData>
      inline bool updatePoint
      (
        const pointData& newPointInfo,
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
        const pointData& pointInfo,
        const scalar tol,
        TrackingData& td
      );
  // Member Operators
    // Needed for List IO
    inline bool operator==(const pointData&) const;
    inline bool operator!=(const pointData&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const pointData&);
    friend Istream& operator>>(Istream&, pointData&);
};
//- Data associated with pointData as contiguous as pointEdgePoint
template<>
inline bool contiguous<pointData>()
{
  return contiguous<pointEdgePoint>();
}
}  // namespace mousse
#include "point_data_i.hpp"
#endif
