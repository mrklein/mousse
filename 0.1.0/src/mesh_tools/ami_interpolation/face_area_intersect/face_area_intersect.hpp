// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceAreaIntersect
// Description
//   Face intersection class
//   - calculates intersection area by sub-dividing face into triangles
//    and cutting
// SourceFiles
//   face_area_intersect.cpp
#ifndef face_area_intersect_hpp_
#define face_area_intersect_hpp_
#include "point_field.hpp"
#include "fixed_list.hpp"
#include "plane.hpp"
#include "face.hpp"
#include "named_enum.hpp"
namespace mousse
{
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
#include "face_area_intersect_i.hpp"
#endif
