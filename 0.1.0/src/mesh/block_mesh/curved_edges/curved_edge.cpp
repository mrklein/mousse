// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "curved_edge.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(curvedEdge, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(curvedEdge, Istream);
}
// Constructors 
mousse::curvedEdge::curvedEdge
(
  const pointField& points,
  const label start,
  const label end
)
:
  points_(points),
  start_(start),
  end_(end)
{}
mousse::curvedEdge::curvedEdge(const pointField& points, Istream& is)
:
  points_(points),
  start_(readLabel(is)),
  end_(readLabel(is))
{}
mousse::curvedEdge::curvedEdge(const curvedEdge& c)
:
  points_(c.points_),
  start_(c.start_),
  end_(c.end_)
{}
mousse::autoPtr<mousse::curvedEdge> mousse::curvedEdge::clone() const
{
  NOT_IMPLEMENTED("curvedEdge::clone() const");
  return autoPtr<curvedEdge>(NULL);
}
mousse::autoPtr<mousse::curvedEdge> mousse::curvedEdge::New
(
  const pointField& points,
  Istream& is
)
{
  if (debug)
  {
    Info<< "curvedEdge::New(const pointField&, Istream&) : "
      << "constructing curvedEdge"
      << endl;
  }
  const word edgeType(is);
  IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(edgeType);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("curvedEdge::New(const pointField&, Istream&)")
      << "Unknown curvedEdge type "
      << edgeType << nl << nl
      << "Valid curvedEdge types are" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<curvedEdge>(cstrIter()(points, is));
}
// Member Functions 
mousse::pointField mousse::curvedEdge::appendEndPoints
(
  const pointField& points,
  const label start,
  const label end,
  const pointField& otherKnots
)
{
  pointField allKnots(otherKnots.size() + 2);
  // Start/end knots
  allKnots[0] = points[start];
  allKnots[otherKnots.size() + 1] = points[end];
  // Intermediate knots
  FOR_ALL(otherKnots, knotI)
  {
    allKnots[knotI+1] = otherKnots[knotI];
  }
  return allKnots;
}
// Member Operators 
void mousse::curvedEdge::operator=(const curvedEdge&)
{
  NOT_IMPLEMENTED("void curvedEdge::operator=(const curvedEdge&)");
}
mousse::Ostream& mousse::operator<<(Ostream& os, const curvedEdge& p)
{
  os << p.start_ << tab << p.end_ << endl;
  return os;
}
