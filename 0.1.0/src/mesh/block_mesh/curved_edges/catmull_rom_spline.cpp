// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "catmull_rom_spline.hpp"
// Constructors 
mousse::CatmullRomSpline::CatmullRomSpline
(
  const pointField& knots,
  const bool closed
)
:
  polyLine(knots, closed)
{}
// Member Functions 
mousse::point mousse::CatmullRomSpline::position(const scalar mu) const
{
  // endpoints
  if (mu < SMALL)
  {
    return points().first();
  }
  else if (mu > 1 - SMALL)
  {
    return points().last();
  }
  scalar lambda = mu;
  label segment = localParameter(lambda);
  return position(segment, lambda);
}
mousse::point mousse::CatmullRomSpline::position
(
  const label segment,
  const scalar mu
) const
{
  // out-of-bounds
  if (segment < 0)
  {
    return points().first();
  }
  else if (segment > nSegments())
  {
    return points().last();
  }
  const point& p0 = points()[segment];
  const point& p1 = points()[segment+1];
  // special cases - no calculation needed
  if (mu <= 0.0)
  {
    return p0;
  }
  else if (mu >= 1.0)
  {
    return p1;
  }
  // determine the end points
  point e0;
  point e1;
  if (segment == 0)
  {
    // end: simple reflection
    e0 = 2*p0 - p1;
  }
  else
  {
    e0 = points()[segment-1];
  }
  if (segment+1 == nSegments())
  {
    // end: simple reflection
    e1 = 2*p1 - p0;
  }
  else
  {
    e1 = points()[segment+2];
  }
  return 0.5 *
  (
    ( 2*p0 )
   + mu *
    (
      ( -e0 + p1 )
     + mu *
      (
        ( 2*e0 - 5*p0 + 4*p1 - e1 )
       + mu *
        ( -e0 + 3*p0 - 3*p1 + e1 )
      )
    )
  );
}
mousse::scalar mousse::CatmullRomSpline::length() const
{
  NOT_IMPLEMENTED("CatmullRomSpline::length() const");
  return 1.0;
}
