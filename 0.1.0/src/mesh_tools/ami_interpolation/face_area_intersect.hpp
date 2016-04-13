#ifndef MESH_TOOLS_AMI_INTERPOLATION_FACE_AREA_INTERSECT_FACE_AREA_INTERSECT_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_FACE_AREA_INTERSECT_FACE_AREA_INTERSECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaIntersect
// Description
//   Face intersection class
//   - calculates intersection area by sub-dividing face into triangles
//    and cutting

#include "point_field.hpp"
#include "fixed_list.hpp"
#include "plane.hpp"
#include "face.hpp"
#include "named_enum.hpp"


namespace mousse {

class faceAreaIntersect
{
public:
  typedef FixedList<point, 3> triPoints;
  enum triangulationMode
  {
    tmFan,
    tmMesh
  };
  static const NamedEnum<triangulationMode, 2> triangulationModeNames_;
private:
  // Private data
    //- Reference to the points of sideA
    const pointField& pointsA_;
    //- Reference to the points of sideB
    const pointField& pointsB_;
    //- Flag to reverse B faces
    const bool reverseB_;
  // Static data members
    static scalar tol;
  // Private Member Functions
    //- Get triPoints from face
    inline triPoints getTriPoints
    (
      const pointField& points,
      const face& f,
      const bool reverse
    ) const;
    //- Set triPoints into tri list
    inline void setTriPoints
    (
      const point& a,
      const point& b,
      const point& c,
      label& count,
      FixedList<triPoints, 10>& tris
    ) const;
    //- Decompose face into triangle fan
    inline void triangleFan
    (
      const face& f,
      DynamicList<face>& faces
    ) const;
    //- Return point of intersection between plane and triangle edge
    inline point planeIntersection
    (
      const FixedList<scalar, 3>& d,
      const triPoints& t,
      const label negI,
      const label posI
    ) const;
    //- Return triangle area
    inline scalar triArea(const triPoints& t) const;
    //- Slice triangle with plane and generate new cut sub-triangles
    void triSliceWithPlane
    (
      const triPoints& tri,
      const plane& p,
      FixedList<triPoints, 10>& tris,
      label& nTris,
      const scalar len
    );
    //- Return area of intersection of triangles src and tgt
    scalar triangleIntersect
    (
      const triPoints& src,
      const triPoints& tgt,
      const vector& n
    );
public:
  // Constructors
    //- Construct from components
    faceAreaIntersect
    (
      const pointField& pointsA,
      const pointField& pointsB,
      const bool reverseB = false
    );
  // Public Member Functions
    //- Fraction of local length scale to use as intersection tolerance
    inline static scalar& tolerance();
    //- Return area of intersection of faceA with faceB
    scalar calc
    (
      const face& faceA,
      const face& faceB,
      const vector& n,
      const triangulationMode& triMode
    );
};

}  // namespace mousse


// Private Member Functions 
inline void mousse::faceAreaIntersect::setTriPoints
(
  const point& a,
  const point& b,
  const point& c,
  label& count,
  FixedList<triPoints, 10>& tris
) const
{
  triPoints& tp = tris[count++];
  tp[0] = a;
  tp[1] = b;
  tp[2] = c;
}


inline mousse::faceAreaIntersect::triPoints mousse::faceAreaIntersect::getTriPoints
(
  const pointField& points,
  const face& f,
  const bool reverse
) const
{
  triPoints result;
  if (reverse) {
    result[2] = points[f[0]];
    result[1] = points[f[1]];
    result[0] = points[f[2]];
  } else {
    result[0] = points[f[0]];
    result[1] = points[f[1]];
    result[2] = points[f[2]];
  }
  return result;
}


inline void mousse::faceAreaIntersect::triangleFan
(
  const face& f,
  DynamicList<face>& faces
) const
{
  if (f.size() < 2)
    return;
  const label v0 = 0;
  labelList indices{3};
  for (label i = 1; i < f.size() - 1; i++) {
    indices[0] = f[v0];
    indices[1] = f[i];
    indices[2] = f[i + 1];
    faces.append(face(indices));
  }
}


inline mousse::point mousse::faceAreaIntersect::planeIntersection
(
  const FixedList<scalar, 3>& d,
  const triPoints& t,
  const label negI,
  const label posI
) const
{
  return (d[posI]*t[negI] - d[negI]*t[posI])/(-d[negI] + d[posI]);
}


inline mousse::scalar mousse::faceAreaIntersect::triArea(const triPoints& t) const
{
  return mag(0.5*((t[1] - t[0])^(t[2] - t[0])));
}


// Member Functions 
mousse::scalar& mousse::faceAreaIntersect::tolerance()
{
  return tol;
}

#endif
