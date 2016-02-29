#ifndef CORE_MESHES_PRIMITIVE_SHAPES_LINE_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_LINE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::line
// Description
//   A line primitive.
#include "vector.hpp"
#include "point_hit.hpp"
#include "point_2d.hpp"
#include "fixed_list.hpp"
#include "ulist.hpp"
#include "iostreams.hpp"
#include "token.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Point, class PointRef> class line;
template<class Point, class PointRef>
inline Istream& operator>>(Istream&, line<Point, PointRef>&);
template<class Point, class PointRef>
inline Ostream& operator<<(Ostream&, const line<Point, PointRef>&);
template<class Point, class PointRef>
class line
{
  // Private data
    PointRef a_, b_;
public:
  // Constructors
    //- Construct from two points
    inline line(const Point& start, const Point& end);
    //- Construct from two points in the list of points
    //  The indices could be from edge etc.
    inline line
    (
      const UList<Point>&,
      const FixedList<label, 2>& indices
    );
    //- Construct from Istream
    inline line(Istream&);
  // Member functions
    // Access
      //- Return first vertex
      inline PointRef start() const;
      //- Return second vertex
      inline PointRef end() const;
    // Properties
      //- Return centre (centroid)
      inline Point centre() const;
      //- Return scalar magnitude
      inline scalar mag() const;
      //- Return start-end vector
      inline Point vec() const;
      //- Return nearest distance to line from a given point
      //  If the nearest point is on the line, return a hit
      PointHit<Point> nearestDist(const Point& p) const;
      //- Return nearest distance from line to line. Returns distance
      //  and sets both points (one on *this, one on the provided
      //  linePointRef.
      scalar nearestDist
      (
        const line<Point, const Point&>& edge,
        Point& thisPoint,
        Point& edgePoint
      ) const;
  // Ostream operator
    friend Istream& operator>> <Point, PointRef>
    (
      Istream&,
      line&
    );
    friend Ostream& operator<< <Point, PointRef>
    (
      Ostream&,
      const line&
    );
};
//- 2D specialisation
template<>
scalar line<point2D, const point2D&>::nearestDist
(
  const line<point2D, const point2D&>& edge,
  point2D& thisPoint,
  point2D& edgePoint
) const;
}  // namespace mousse

// Constructors
template<class Point, class PointRef>
inline mousse::line<Point, PointRef>::line(const Point& start, const Point& end)
:
  a_{start},
  b_{end}
{}
template<class Point, class PointRef>
inline mousse::line<Point, PointRef>::line
(
  const UList<Point>& points,
  const FixedList<label, 2>& indices
)
:
  a_{points[indices[0]]},
  b_{points[indices[1]]}
{}
template<class Point, class PointRef>
inline mousse::line<Point, PointRef>::line(Istream& is)
{
  is  >> *this;
}
// Member Functions
template<class Point, class PointRef>
inline PointRef mousse::line<Point, PointRef>::start() const
{
  return a_;
}
template<class Point, class PointRef>
inline PointRef mousse::line<Point, PointRef>::end() const
{
  return b_;
}
template<class Point, class PointRef>
inline Point mousse::line<Point, PointRef>::centre() const
{
  return 0.5*(a_ + b_);
}
template<class Point, class PointRef>
inline mousse::scalar mousse::line<Point, PointRef>::mag() const
{
  return ::mousse::mag(vec());
}
template<class Point, class PointRef>
inline Point mousse::line<Point, PointRef>::vec() const
{
  return b_ - a_;
}
template<class Point, class PointRef>
mousse::PointHit<Point> mousse::line<Point, PointRef>::nearestDist
(
  const Point& p
) const
{
  Point v = vec();
  Point w(p - a_);
  scalar c1 = v & w;
  if (c1 <= 0)
  {
    return PointHit<Point>(false, a_, mousse::mag(p - a_), true);
  }
  scalar c2 = v & v;
  if (c2 <= c1)
  {
    return PointHit<Point>(false, b_, mousse::mag(p - b_), true);
  }
  scalar b = c1/c2;
  Point pb(a_ + b*v);
  return {true, pb, mousse::mag(p - pb), false};
}
template<class Point, class PointRef>
mousse::scalar mousse::line<Point, PointRef>::nearestDist
(
  const line<Point, const Point&>& edge,
  Point& thisPt,
  Point& edgePt
) const
{
  // From Mathworld Line-Line distance/(Gellert et al. 1989, p. 538).
  Point a(end() - start());
  Point b(edge.end() - edge.start());
  Point c(edge.start() - start());
  Point crossab = a ^ b;
  scalar magCrossSqr = magSqr(crossab);
  if (magCrossSqr > VSMALL)
  {
    scalar s = ((c ^ b) & crossab)/magCrossSqr;
    scalar t = ((c ^ a) & crossab)/magCrossSqr;
    // Check for end points outside of range 0..1
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
      // Both inside range 0..1
      thisPt = start() + a*s;
      edgePt = edge.start() + b*t;
    }
    else
    {
      // Do brute force. Distance of everything to everything.
      // Can quite possibly be improved!
      // From edge endpoints to *this
      PointHit<Point> this0(nearestDist(edge.start()));
      PointHit<Point> this1(nearestDist(edge.end()));
      scalar thisDist = min(this0.distance(), this1.distance());
      // From *this to edge
      PointHit<Point> edge0(edge.nearestDist(start()));
      PointHit<Point> edge1(edge.nearestDist(end()));
      scalar edgeDist = min(edge0.distance(), edge1.distance());
      if (thisDist < edgeDist)
      {
        if (this0.distance() < this1.distance())
        {
          thisPt = this0.rawPoint();
          edgePt = edge.start();
        }
        else
        {
          thisPt = this1.rawPoint();
          edgePt = edge.end();
        }
      }
      else
      {
        if (edge0.distance() < edge1.distance())
        {
          thisPt = start();
          edgePt = edge0.rawPoint();
        }
        else
        {
          thisPt = end();
          edgePt = edge1.rawPoint();
        }
      }
    }
  }
  else
  {
    // Parallel lines. Find overlap of both lines by projecting onto
    // direction vector (now equal for both lines).
    scalar edge0 = edge.start() & a;
    scalar edge1 = edge.end() & a;
    bool edgeOrder = edge0 < edge1;
    scalar minEdge = (edgeOrder ? edge0 : edge1);
    scalar maxEdge = (edgeOrder ? edge1 : edge0);
    const Point& minEdgePt = (edgeOrder ? edge.start() : edge.end());
    const Point& maxEdgePt = (edgeOrder ? edge.end() : edge.start());
    scalar this0 = start() & a;
    scalar this1 = end() & a;
    bool thisOrder = this0 < this1;
    scalar minThis = min(this0, this1);
    scalar maxThis = max(this1, this0);
    const Point& minThisPt = (thisOrder ? start() : end());
    const Point& maxThisPt = (thisOrder ? end() : start());
    if (maxEdge < minThis)
    {
      // edge completely below *this
      edgePt = maxEdgePt;
      thisPt = minThisPt;
    }
    else if (maxEdge < maxThis)
    {
      // maxEdge inside interval of *this
      edgePt = maxEdgePt;
      thisPt = nearestDist(edgePt).rawPoint();
    }
    else
    {
      // maxEdge outside. Check if minEdge inside.
      if (minEdge < minThis)
      {
        // Edge completely envelops this. Take any this point and
        // determine nearest on edge.
        thisPt = minThisPt;
        edgePt = edge.nearestDist(thisPt).rawPoint();
      }
      else if (minEdge < maxThis)
      {
        // minEdge inside this interval.
        edgePt = minEdgePt;
        thisPt = nearestDist(edgePt).rawPoint();
      }
      else
      {
        // minEdge outside this interval
        edgePt = minEdgePt;
        thisPt = maxThisPt;
      }
    }
  }
  return mousse::mag(thisPt - edgePt);
}
// IOstream Operators
template<class Point, class PointRef>
inline mousse::Istream& mousse::operator>>
(
  Istream& is,
  line<Point, PointRef>& l
)
{
  is.readBegin("line");
  is  >> l.a_ >> l.b_;
  is.readEnd("line");
  is.check("Istream& operator>>(Istream&, line&)");
  return is;
}
template<class Point, class PointRef>
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const line<Point, PointRef>& l
)
{
  os<< token::BEGIN_LIST
    << l.a_ << token::SPACE
    << l.b_
    << token::END_LIST;
  return os;
}
#endif
