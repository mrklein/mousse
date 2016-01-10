// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pyramid
// Description
//   A geometric pyramid primitive with a base of 'n' sides:
//   i.e. a parametric pyramid. A pyramid is constructed from
//   a base polygon and an apex point.
// SourceFiles
//   pyramid_i.hpp
#ifndef pyramid_hpp_
#define pyramid_hpp_
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
#include "pyramid_i.hpp"
#endif
