#ifndef MESH_TOOLS_MESH_STRUCTURE_POINT_TOPO_DISTANCE_DATA_HPP_
#define MESH_TOOLS_MESH_STRUCTURE_POINT_TOPO_DISTANCE_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointTopoDistanceData
// Description
//   For use with PointEdgeWave. Determines topological distance to
//   starting points
// SourceFiles
//   point_topo_distance_data.cpp
#include "point.hpp"
#include "tensor.hpp"
#include "poly_mesh.hpp"
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

// Private Member Functions 
// Constructors 
// Null constructor
inline mousse::pointTopoDistanceData::pointTopoDistanceData()
:
  data_{-1},
  distance_{-1}
{}
// Construct from components
inline mousse::pointTopoDistanceData::pointTopoDistanceData
(
  const label data,
  const label distance
)
:
  data_{data},
  distance_{distance}
{}
// Member Functions 
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::valid(TrackingData&) const
{
  return distance_ != -1;
}
// No geometric data so never any problem on cyclics
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::sameGeometry
(
  const pointTopoDistanceData&,
  const scalar /*tol*/,
  TrackingData&
) const
{
  return true;
}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::leaveDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& /*coord*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::transform
(
  const tensor& /*rotTensor*/,
  TrackingData&
)
{}
// No geometric data.
template<class TrackingData>
inline void mousse::pointTopoDistanceData::enterDomain
(
  const polyPatch&,
  const label /*patchPointI*/,
  const point& /*coord*/,
  TrackingData&
)
{}
// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const label /*edgeI*/,
  const pointTopoDistanceData& edgeInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    data_ = edgeInfo.data_;
    distance_ = edgeInfo.distance_ + 1;
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const polyMesh&,
  const label /*pointI*/,
  const pointTopoDistanceData& newPointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(newPointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updatePoint
(
  const pointTopoDistanceData& newPointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(newPointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::updateEdge
(
  const polyMesh&,
  const label /*edgeI*/,
  const label /*pointI*/,
  const pointTopoDistanceData& pointInfo,
  const scalar /*tol*/,
  TrackingData&
)
{
  if (distance_ == -1)
  {
    operator=(pointInfo);
    return true;
  }
  else
  {
    return false;
  }
}
template<class TrackingData>
inline bool mousse::pointTopoDistanceData::equal
(
  const pointTopoDistanceData& rhs,
  TrackingData&
) const
{
  return operator==(rhs);
}
// Member Operators 
inline bool mousse::pointTopoDistanceData::operator==
(
  const mousse::pointTopoDistanceData& rhs
) const
{
  return data() == rhs.data() && distance() == rhs.distance();
}
inline bool mousse::pointTopoDistanceData::operator!=
(
  const mousse::pointTopoDistanceData& rhs
) const
{
  return !(*this == rhs);
}
#endif
