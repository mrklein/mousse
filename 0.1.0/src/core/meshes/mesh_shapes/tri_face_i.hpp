// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iostreams.hpp"
#include "face.hpp"
#include "tri_point_ref.hpp"
#include "swap.hpp"
// Static Member Functions
inline int mousse::triFace::compare(const triFace& a, const triFace& b)
{
  if
  (
    (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
  || (a[0] == b[1] && a[1] == b[2] && a[2] == b[0])
  || (a[0] == b[2] && a[1] == b[0] && a[2] == b[1])
  )
  {
    // identical
    return 1;
  }
  else if
  (
    (a[0] == b[2] && a[1] == b[1] && a[2] == b[0])
  || (a[0] == b[1] && a[1] == b[0] && a[2] == b[2])
  || (a[0] == b[0] && a[1] == b[2] && a[2] == b[1])
  )
  {
    // same face, but reversed orientation
    return -1;
  }
  else
  {
    return 0;
  }
}
// Constructors 
inline mousse::triFace::triFace()
{}
inline mousse::triFace::triFace
(
  const label a,
  const label b,
  const label c
)
{
  operator[](0) = a;
  operator[](1) = b;
  operator[](2) = c;
}
inline mousse::triFace::triFace(const labelUList& lst)
:
  FixedList<label, 3>(lst)
{}
inline mousse::triFace::triFace(Istream& is)
:
  FixedList<label, 3>(is)
{}
// Member Functions 
inline mousse::label mousse::triFace::collapse()
{
  // we cannot resize a FixedList, so mark duplicates with '-1'
  // (the lower vertex is retained)
  // catch any '-1' (eg, if called twice)
  label n = 3;
  if (operator[](0) == operator[](1) || operator[](1) == -1)
  {
    operator[](1) = -1;
    n--;
  }
  else if (operator[](1) == operator[](2) || operator[](2) == -1)
  {
    operator[](2) = -1;
    n--;
  }
  if (operator[](0) == operator[](2))
  {
    operator[](2) = -1;
    n--;
  }
  return n;
}
inline void mousse::triFace::flip()
{
  Swap(operator[](1), operator[](2));
}
inline mousse::pointField mousse::triFace::points(const pointField& points) const
{
  pointField p(3);
  p[0] = points[operator[](0)];
  p[1] = points[operator[](1)];
  p[2] = points[operator[](2)];
  return p;
}
inline mousse::face mousse::triFace::triFaceFace() const
{
  mousse::face f(3);
  f[0] = operator[](0);
  f[1] = operator[](1);
  f[2] = operator[](2);
  return f;
}
inline mousse::triPointRef mousse::triFace::tri(const pointField& points) const
{
  return triPointRef
  (
    points[operator[](0)],
    points[operator[](1)],
    points[operator[](2)]
  );
}
inline mousse::point mousse::triFace::centre(const pointField& points) const
{
  return (1.0/3.0)*
  (
    points[operator[](0)]
   + points[operator[](1)]
   + points[operator[](2)]
  );
}
inline mousse::scalar mousse::triFace::mag(const pointField& points) const
{
  return ::mousse::mag(normal(points));
}
// could also delegate to triPointRef(...).normal()
inline mousse::vector mousse::triFace::normal(const pointField& points) const
{
  return 0.5*
  (
    (points[operator[](1)] - points[operator[](0)])
   ^(points[operator[](2)] - points[operator[](0)])
  );
}
inline mousse::label mousse::triFace::nTriangles() const
{
  return 1;
}
inline mousse::triFace mousse::triFace::reverseFace() const
{
  // The starting points of the original and reverse face are identical.
  return triFace(operator[](0), operator[](2), operator[](1));
}
inline mousse::scalar mousse::triFace::sweptVol
(
  const pointField& opts,
  const pointField& npts
) const
{
  return (1.0/6.0)*
  (
    (
      (npts[operator[](0)] - opts[operator[](0)])
     & (
        (opts[operator[](1)] - opts[operator[](0)])
       ^ (opts[operator[](2)] - opts[operator[](0)])
      )
    )
   + (
      (npts[operator[](1)] - opts[operator[](1)])
     & (
        (opts[operator[](2)] - opts[operator[](1)])
       ^ (npts[operator[](0)] - opts[operator[](1)])
      )
    )
   + (
      (opts[operator[](2)] - npts[operator[](2)])
     & (
        (npts[operator[](1)] - npts[operator[](2)])
       ^ (npts[operator[](0)] - npts[operator[](2)])
      )
    )
  );
}
mousse::tensor mousse::triFace::inertia
(
  const pointField& points,
  const point& refPt,
  scalar density
) const
{
  // a triangle, do a direct calculation
  return this->tri(points).inertia(refPt, density);
}
inline mousse::pointHit mousse::triFace::ray
(
  const point& p,
  const vector& q,
  const pointField& points,
  const intersection::algorithm alg,
  const intersection::direction dir
) const
{
  return this->tri(points).ray(p, q, alg, dir);
}
inline mousse::pointHit mousse::triFace::intersection
(
  const point& p,
  const vector& q,
  const pointField& points,
  const intersection::algorithm alg,
  const scalar tol
) const
{
  return this->tri(points).intersection(p, q, alg, tol);
}
inline mousse::pointHit mousse::triFace::intersection
(
  const point& p,
  const vector& q,
  const point& ctr,
  const pointField& points,
  const intersection::algorithm alg,
  const scalar tol
) const
{
  return intersection(p, q, points, alg, tol);
}
inline mousse::pointHit mousse::triFace::nearestPoint
(
  const point& p,
  const pointField& points
) const
{
  return this->tri(points).nearestPoint(p);
}
inline mousse::pointHit mousse::triFace::nearestPointClassify
(
  const point& p,
  const pointField& points,
  label& nearType,
  label& nearLabel
) const
{
  return this->tri(points).nearestPointClassify(p, nearType, nearLabel);
}
inline mousse::label mousse::triFace::nEdges() const
{
  return 3;
}
inline mousse::edgeList mousse::triFace::edges() const
{
  edgeList e(3);
  e[0].start() = operator[](0);
  e[0].end()   = operator[](1);
  e[1].start() = operator[](1);
  e[1].end()   = operator[](2);
  e[2].start() = operator[](2);
  e[2].end()   = operator[](0);
  return e;
}
inline mousse::edge mousse::triFace::faceEdge(const label n) const
{
  return edge(operator[](n), operator[](fcIndex(n)));
}
// return
//  - +1: forward (counter-clockwise) on the face
//  - -1: reverse (clockwise) on the face
//  -  0: edge not found on the face
inline int mousse::triFace::edgeDirection(const edge& e) const
{
  if
  (
    (operator[](0) == e.start() && operator[](1) == e.end())
  || (operator[](1) == e.start() && operator[](2) == e.end())
  || (operator[](2) == e.start() && operator[](0) == e.end())
  )
  {
    return 1;
  }
  else if
  (
    (operator[](0) == e.end() && operator[](1) == e.start())
  || (operator[](1) == e.end() && operator[](2) == e.start())
  || (operator[](2) == e.end() && operator[](0) == e.start())
  )
  {
    return -1;
  }
  else
  {
    return 0;
  }
}
// Friend Operators 
inline bool mousse::operator==(const triFace& a, const triFace& b)
{
  return triFace::compare(a,b) != 0;
}
inline bool mousse::operator!=(const triFace& a, const triFace& b)
{
  return triFace::compare(a,b) == 0;
}
