#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_AUTO_HEX_MESH_DRIVER_POINT_DATA_POINT_DATA_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_AUTO_HEX_MESH_DRIVER_POINT_DATA_POINT_DATA_HPP_

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

#include "point_edge_point.hpp"
#include "poly_mesh.hpp"
#include "transform.hpp"


namespace mousse {

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
inline bool contiguous<pointData>() { return contiguous<pointEdgePoint>(); }

}  // namespace mousse


// Constructors 
// Null constructor
inline mousse::pointData::pointData()
:
  pointEdgePoint{},
  s_{GREAT},
  v_{point::max}
{}


// Construct from origin, distance
inline mousse::pointData::pointData
(
  const point& origin,
  const scalar distSqr,
  const scalar s,
  const vector& v
)
:
  pointEdgePoint{origin, distSqr},
  s_{s},
  v_{v}
{}


// Construct as copy
inline mousse::pointData::pointData(const pointData& wpt)
:
  pointEdgePoint{wpt},
  s_{wpt.s()},
  v_{wpt.v()}
{}


// Member Functions 
inline mousse::scalar mousse::pointData::s() const
{
  return s_;
}


inline const mousse::vector& mousse::pointData::v() const
{
  return v_;
}


template<class TrackingData>
inline void mousse::pointData::transform
(
  const tensor& rotTensor,
  TrackingData& td
)
{
  pointEdgePoint::transform(rotTensor, td);
  v_ = mousse::transform(rotTensor, v_);
}


// Update this with information from connected edge
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const label edgeI,
  const pointData& edgeInfo,
  const scalar tol,
  TrackingData& td
)
{
  const bool uP = 
    pointEdgePoint::updatePoint(mesh, pointI, edgeI, edgeInfo, tol, td);
  if (uP) {
    s_ = edgeInfo.s_;
    v_ = edgeInfo.v_;
    return true;
  }
  return false;
}


// Update this with new information on same point
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const polyMesh& mesh,
  const label pointI,
  const pointData& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  const bool uP = 
    pointEdgePoint::updatePoint(mesh, pointI, newPointInfo, tol, td);
  if (uP) {
    s_ = newPointInfo.s_;
    v_ = newPointInfo.v_;
    return true;
  }
  return false;
}


// Update this with new information on same point. No extra information.
template<class TrackingData>
inline bool mousse::pointData::updatePoint
(
  const pointData& newPointInfo,
  const scalar tol,
  TrackingData& td
)
{
  if (pointEdgePoint::updatePoint(newPointInfo, tol, td)) {
    s_ = newPointInfo.s_;
    v_ = newPointInfo.v_;
    return true;
  }
  return false;
}


// Update this with information from connected point
template<class TrackingData>
inline bool mousse::pointData::updateEdge
(
  const polyMesh& mesh,
  const label edgeI,
  const label pointI,
  const pointData& pointInfo,
  const scalar tol,
  TrackingData& td
)
{
  const bool uE = 
    pointEdgePoint::updateEdge(mesh, edgeI, pointI, pointInfo, tol, td);
  if (uE) {
    s_ = pointInfo.s_;
    v_ = pointInfo.v_;
    return true;
  }
  return false;
}


// Member Operators 
inline bool mousse::pointData::operator==(const mousse::pointData& rhs)
const
{
  return (pointEdgePoint::operator==(rhs)
          && (s() == rhs.s())
          && (v() == rhs.v()));
}


inline bool mousse::pointData::operator!=(const mousse::pointData& rhs)
const
{
  return !(*this == rhs);
}

#endif

