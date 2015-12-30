// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nearest_to_point.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(nearestToPoint, 0);
addToRunTimeSelectionTable(topoSetSource, nearestToPoint, word);
addToRunTimeSelectionTable(topoSetSource, nearestToPoint, istream);
}
mousse::topoSetSource::addToUsageTable mousse::nearestToPoint::usage_
(
  nearestToPoint::typeName,
  "\n    Usage: nearestToPoint (pt0 .. ptn)\n\n"
  "    Select the nearest point for each of the points pt0 ..ptn\n\n"
);
// Private Member Functions 
void mousse::nearestToPoint::combine(topoSet& set, const bool add) const
{
  // Do linear search since usually just a few points.
  forAll(points_, pointI)
  {
    const pointField& pts = mesh_.points();
    if (pts.size())
    {
      label minPointI = 0;
      scalar minDistSqr = magSqr(pts[minPointI] - points_[pointI]);
      for (label i = 1; i < pts.size(); i++)
      {
        scalar distSqr = magSqr(pts[i] - points_[pointI]);
        if (distSqr < minDistSqr)
        {
          minDistSqr = distSqr;
          minPointI = i;
        }
      }
      addOrDelete(set, minPointI, add);
    }
  }
}
// Constructors 
// Construct from components
mousse::nearestToPoint::nearestToPoint
(
  const polyMesh& mesh,
  const pointField& points
)
:
  topoSetSource(mesh),
  points_(points)
{}
// Construct from dictionary
mousse::nearestToPoint::nearestToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  points_(dict.lookup("points"))
{}
// Construct from Istream
mousse::nearestToPoint::nearestToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  points_(checkIs(is))
{}
// Destructor 
mousse::nearestToPoint::~nearestToPoint()
{}
// Member Functions 
void mousse::nearestToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding points nearest to " << points_ << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing points nearest to " << points_ << endl;
    combine(set, false);
  }
}
