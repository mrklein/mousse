// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointTopoDistanceData
// Description
//   For use with PointEdgeWave. Determines topological distance to
//   starting points
// SourceFiles
//   point_topo_distance_data_i.hpp
//   point_topo_distance_data.cpp
#ifndef point_topo_distance_data_hpp_
#define point_topo_distance_data_hpp_
#include "point.hpp"
#include "tensor.hpp"
namespace mousse
{
class polyPatch;
class polyMesh;
class pointTopoDistanceData
{
  // Private data
    //- Starting data
    label data_;
    //- Distance
    label distance_;
public:
  // Constructors
    //- Construct null
    inline pointTopoDistanceData();
    //- Construct from count
    inline pointTopoDistanceData
    (
      const label data,
      const label distance
    );
  // Member Functions
    // Access
      inline label data() const
      {
        return data_;
      }
      inline label distance() const
      {
        return distance_;
      }
    // Needed by PointEdgeWave
      //- Check whether origin has been changed at all or
      //  still contains original (invalid) value.
      template<class TrackingData>
      inline bool valid(TrackingData& td) const;
      //- Check for identical geometrical data. Used for cyclics checking.
      template<class TrackingData>
      inline bool sameGeometry
      (
        const pointTopoDistanceData&,
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
        const pointTopoDistanceData& edgeInfo,
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
        const pointTopoDistanceData& newPointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Influence of different value on same point.
      //  No information about current position whatsoever.
      template<class TrackingData>
      inline bool updatePoint
      (
        const pointTopoDistanceData& newPointInfo,
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
        const pointTopoDistanceData& pointInfo,
        const scalar tol,
        TrackingData& td
      );
      //- Same (like operator==)
      template<class TrackingData>
      inline bool equal(const pointTopoDistanceData&, TrackingData&)
      const;
  // Member Operators
    // Needed for List IO
    inline bool operator==(const pointTopoDistanceData&) const;
    inline bool operator!=(const pointTopoDistanceData&) const;
  // IOstream Operators
    friend Ostream& operator<<(Ostream&, const pointTopoDistanceData&);
    friend Istream& operator>>(Istream&, pointTopoDistanceData&);
};
//- Data associated with pointTopoDistanceData type are contiguous
template<>
inline bool contiguous<pointTopoDistanceData>()
{
  return true;
}
}  // namespace mousse
#include "point_topo_distance_data_i.hpp"
#endif
