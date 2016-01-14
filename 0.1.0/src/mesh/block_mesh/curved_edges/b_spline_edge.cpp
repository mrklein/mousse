// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "b_spline_edge.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(BSplineEdge, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    curvedEdge,
    BSplineEdge,
    Istream
  );
}
// Constructors 
mousse::BSplineEdge::BSplineEdge
(
  const pointField& points,
  const label start,
  const label end,
  const pointField& internalPoints
)
:
  curvedEdge(points, start, end),
  BSpline(appendEndPoints(points, start, end, internalPoints))
{}
mousse::BSplineEdge::BSplineEdge(const pointField& points, Istream& is)
:
  curvedEdge(points, is),
  BSpline(appendEndPoints(points, start_, end_, pointField(is)))
{
  token t(is);
  is.putBack(t);
  // discard unused start/end tangents
  if (t == token::BEGIN_LIST)
  {
    vector tangent0Ignored(is);
    vector tangent1Ignored(is);
  }
}
// Destructor 
mousse::BSplineEdge::~BSplineEdge()
{}
// Member Functions 
mousse::point mousse::BSplineEdge::position(const scalar mu) const
{
  return BSpline::position(mu);
}
mousse::scalar mousse::BSplineEdge::length() const
{
  return BSpline::length();
}
