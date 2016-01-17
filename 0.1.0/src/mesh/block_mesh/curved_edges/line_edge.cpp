// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "line_edge.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(lineEdge, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(curvedEdge, lineEdge, Istream);
}
// Constructors 
mousse::lineEdge::lineEdge
(
  const pointField& points,
  const label start,
  const label end
)
:
  curvedEdge(points, start, end)
{}
mousse::lineEdge::lineEdge(const pointField& points, Istream& is)
:
  curvedEdge(points, is)
{}
// Destructor
mousse::lineEdge::~lineEdge()
{}
// Member Functions 
mousse::point mousse::lineEdge::position(const scalar lambda) const
{
  if (lambda < -SMALL || lambda > 1+SMALL)
  {
    FATAL_ERROR_IN("lineEdge::position(const scalar)")
      << "Parameter out of range, lambda = " << lambda
      << abort(FatalError);
  }
  return points_[start_] + lambda * (points_[end_] - points_[start_]);
}
mousse::scalar mousse::lineEdge::length() const
{
  return mag(points_[end_] - points_[start_]);
}
