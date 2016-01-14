// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spline_edge.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(splineEdge, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    curvedEdge,
    splineEdge,
    Istream
  );
}
// Constructors 
mousse::splineEdge::splineEdge
(
  const pointField& points,
  const label start,
  const label end,
  const pointField& internalPoints
)
:
  curvedEdge(points, start, end),
  CatmullRomSpline(appendEndPoints(points, start, end, internalPoints))
{}
mousse::splineEdge::splineEdge(const pointField& points, Istream& is)
:
  curvedEdge(points, is),
  CatmullRomSpline(appendEndPoints(points, start_, end_, pointField(is)))
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
mousse::splineEdge::~splineEdge()
{}
// Member Functions 
mousse::point mousse::splineEdge::position(const scalar mu) const
{
  return CatmullRomSpline::position(mu);
}
mousse::scalar mousse::splineEdge::length() const
{
  return CatmullRomSpline::length();
}
