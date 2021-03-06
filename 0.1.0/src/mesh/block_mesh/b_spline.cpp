// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "b_spline.hpp"


// Constructors 
mousse::BSpline::BSpline
(
  const pointField& knots,
  const bool closed
)
:
  polyLine{knots, closed}
{}


// Member Functions 
mousse::point mousse::BSpline::position(const scalar mu) const
{
  // endpoints
  if (mu < SMALL) {
    return points().first();
  } else if (mu > 1 - SMALL) {
    return points().last();
  }
  scalar lambda = mu;
  label segment = localParameter(lambda);
  return position(segment, lambda);
}


mousse::point mousse::BSpline::position
(
  const label segment,
  const scalar mu
) const
{
  // out-of-bounds
  if (segment < 0) {
    return points().first();
  } else if (segment > nSegments()) {
    return points().last();
  }
  const point& p0 = points()[segment];
  const point& p1 = points()[segment+1];
  // special cases - no calculation needed
  if (mu <= 0.0) {
    return p0;
  } else if (mu >= 1.0) {
    return p1;
  }
  // determine the end points
  point e0;
  point e1;
  if (segment == 0) {
    // end: simple reflection
    e0 = 2*p0 - p1;
  } else {
    e0 = points()[segment-1];
  }
  if (segment + 1 == nSegments()) {
    // end: simple reflection
    e1 = 2*p1 - p0;
  } else {
    e1 = points()[segment+2];
  }
  return ((e0 + 4*p0 + p1)
          + mu *((-3*e0 + 3*p1)
                 + mu*((3*e0 - 6*p0 + 3*p1)
                       + mu*(-e0 + 3*p0 - 3*p1 + e1))))/6.0;
}


mousse::scalar mousse::BSpline::length() const
{
  NOT_IMPLEMENTED("BSpline::length() const");
  return 1.0;
}

