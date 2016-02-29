#ifndef TRI_SURFACE_FACE_TRIANGULATION_HPP_
#define TRI_SURFACE_FACE_TRIANGULATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceTriangulation
// Description
//   Triangulation of faces. Handles concave polygons as well
//   (inefficiently)
//   Works by trying to subdivide the face at the vertex with 'flattest'
//   internal angle (i.e. closest to 180 deg).
//   Based on routine 'Diagonal' in
//   \verbatim
//     "Efficient Triangulation of Simple Polygons"
//     Godfried Toussaint, McGill University.
//   \endverbatim
//   After construction is the list of triangles the face is decomposed into.
//   (Or empty list if no valid triangulation could be found).
// SourceFiles
//   face_triangulation.cpp
#include "tri_face_list.hpp"
#include "point_field.hpp"
namespace mousse
{
// Forward declaration of classes
class faceTriangulation
:
  public triFaceList
{
  // Static Data
    //- Relative tolerance on edge.
    static const scalar edgeRelTol;
  // Static Member Functions
    //- Edge to the right of face vertex i
    static label right(const label size, label i);
    //- Edge to the left of face vertex i
    static label left(const label size, label i);
    //- Calculate normalized edge vectors
    static tmp<vectorField> calcEdges(const face&, const pointField&);
    //- Calculates half angle components of angle from e0 to e1
    //  in plane given by normal.
    static void calcHalfAngle
    (
      const vector& normal,
      const vector& e0,
      const vector& e1,
      scalar& cosHalfAngle,
      scalar& sinHalfAngle
    );
    //- Calculate intersection point between edge p1-p2 and ray (in 2D).
    // Return true and intersection point if intersection between p1 and p2.
    static pointHit rayEdgeIntersect
    (
      const vector& normal,
      const point& rayOrigin,
      const vector& rayDir,
      const point& p1,
      const point& p2,
      scalar& posOnEdge
    );
    // Return true if triangle given its three points
    // (anticlockwise ordered) contains point
    static bool triangleContainsPoint
    (
      const vector& n,
      const point& p0,
      const point& p1,
      const point& p2,
      const point& pt
    );
    //- Starting from startIndex find diagonal. Return in index1, index2.
    //  Index1 always startIndex except when convex polygon
    static void findDiagonal
    (
      const pointField& points,
      const face& f,
      const vectorField& edges,
      const vector& normal,
      const label startIndex,
      label& index1,
      label& index2
    );
    //- Find label of vertex to start splitting from. This will be the
    //  vertex with edge angle:
    //     1] flattest concave angle
    //     2] flattest convex angle if no concave angles.
    static label findStart
    (
      const face& f,
      const vectorField& edges,
      const vector& normal
    );
  // Private Member Functions
    //- Split face f into triangles. Handles all simple (convex & concave)
    //  polygons. Returns false if could not produce valid split.
    bool split
    (
      const bool fallBack,
      const pointField& points,
      const face& f,
      const vector& normal,
      label& triI
    );
public:
  // Constructors
    //- Construct null
    faceTriangulation();
    //- Construct from face and points. Decomposition based on average
    //  normal. After construction *this is size 0 or holds the triangles.
    //  If fallBack and triangulation fails does naive triangulation
    //  and never returns 0 size.
    faceTriangulation
    (
      const pointField& points,
      const face& f,
      const bool fallBack = false
    );
    //- Construct from face and points and user supplied (unit) normal.
    //  After construction *this is size 0 or holds the triangles.
    //  If fallBack and triangulation fails does naive triangulation
    //  and never returns 0 size.
    faceTriangulation
    (
      const pointField& points,
      const face& f,
      const vector& n,
      const bool fallBack = false
    );
    //- Construct from Istream
    faceTriangulation(Istream&);
};
}  // namespace mousse
#endif
