// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "poly_line_edge.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(polyLineEdge, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(curvedEdge, polyLineEdge, Istream);
}
// Constructors 
mousse::polyLineEdge::polyLineEdge
(
  const pointField& ps,
  const label start,
  const label end,
  const pointField& otherPoints
)
:
  curvedEdge(ps, start, end),
  polyLine(appendEndPoints(ps, start_, end_, otherPoints))
{}
mousse::polyLineEdge::polyLineEdge(const pointField& ps, Istream& is)
:
  curvedEdge(ps, is),
  polyLine(appendEndPoints(ps, start_, end_, pointField(is)))
{}
// Destructor 
mousse::polyLineEdge::~polyLineEdge()
{}
// Member Functions 
mousse::point mousse::polyLineEdge::position(const scalar lambda) const
{
  return polyLine::position(lambda);
}
mousse::scalar mousse::polyLineEdge::length() const
{
  return polyLine::lineLength_;
}
