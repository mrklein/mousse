// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyramid
// Description
//   A geometric pyramid primitive with a base of 'n' sides:
//   i.e. a parametric pyramid. A pyramid is constructed from
//   a base polygon and an apex point.
#ifndef pyramid_hpp_
#define pyramid_hpp_
#include "iostreams.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Point, class PointRef, class polygonRef>
class pyramid;
template<class Point, class PointRef, class polygonRef>
inline Istream& operator>>
(
  Istream&,
  pyramid<Point, PointRef, polygonRef>&
);
template<class Point, class PointRef, class polygonRef>
inline Ostream& operator<<
(
  Ostream&,
  const pyramid<Point, PointRef, polygonRef>&
);
template<class Point, class PointRef, class polygonRef>
class pyramid
{
  // Private data
    polygonRef base_;
    PointRef apex_;
public:
  // Constructors
    //- Construct from base polygon and apex point
    inline pyramid(polygonRef base, const Point& apex);
    //- Construct from Istream
    inline pyramid(Istream&);
  // Member functions
    // Access
      //- Return apex point
      inline const Point& apex() const;
      //- Return base polygon
      inline polygonRef base() const;
    // Properties
      //- Return centre (centroid)
      inline Point centre(const pointField& points) const;
      //- Return height vector
      inline vector height(const pointField& points) const;
      //- Return scalar magnitude - returns volume of pyramid
      inline scalar mag(const pointField& points) const;
  // IOstream operators
    friend Istream& operator>> <Point, PointRef, polygonRef>
    (
      Istream&,
      pyramid&
    );
    friend Ostream& operator<< <Point, PointRef, polygonRef>
    (
      Ostream&,
      const pyramid&
    );
};
}  // namespace mousse

// Constructors 
template<class Point, class PointRef, class polygonRef>
inline mousse::pyramid<Point, PointRef, polygonRef>::pyramid
(
  polygonRef base,
  const Point& apex
)
:
  base_(base),
  apex_(apex)
{}
template<class Point, class PointRef, class polygonRef>
inline mousse::pyramid<Point, PointRef, polygonRef>::pyramid(Istream& is)
{
  is >> base_ >> apex_;
  is.check("pyramid::pyramid(Istream&)");
}
// Member Functions 
template<class Point, class PointRef, class polygonRef>
inline const Point& mousse::pyramid<Point, PointRef, polygonRef>::apex() const
{
  return apex_;
}
template<class Point, class PointRef, class polygonRef>
inline polygonRef mousse::pyramid<Point, PointRef, polygonRef>::base() const
{
  return base_;
}
template<class Point, class PointRef, class polygonRef>
inline Point mousse::pyramid<Point, PointRef, polygonRef>::centre
(
  const pointField& points
) const
{
  return (3.0/4.0)*base_.centre(points) + (1.0/4.0)*apex_;
}
template<class Point, class PointRef, class polygonRef>
inline mousse::vector mousse::pyramid<Point, PointRef, polygonRef>::height
(
  const pointField& points
) const
{
  // Height = apex - baseCentroid
  return (apex_ - base_.centre(points));
}
template<class Point, class PointRef, class polygonRef>
inline mousse::scalar mousse::pyramid<Point, PointRef, polygonRef>::mag
(
  const pointField& points
) const
{
  return (1.0/3.0)*(base_.normal(points)&(height(points)));
}
// IOstream Operators 
template<class Point, class PointRef, class polygonRef>
inline mousse::Istream& mousse::operator>>
(
  Istream& is,
  pyramid<Point, PointRef, polygonRef>& p
)
{
  is  >> p.base_ >> p.apex_;
  is.check("Istream& operator>>(Istream&, pyramid&)");
  return is;
}
template<class Point, class PointRef, class polygonRef>
inline mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const pyramid<Point, PointRef, polygonRef>& p
)
{
  os  << p.base_ << tab << p.apex_ << nl;
  return os;
}
#endif
