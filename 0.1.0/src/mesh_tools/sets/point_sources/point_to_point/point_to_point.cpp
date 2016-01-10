// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_to_point.hpp"
#include "poly_mesh.hpp"
#include "point_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(pointToPoint, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, pointToPoint, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, pointToPoint, istream);
}

mousse::topoSetSource::addToUsageTable mousse::pointToPoint::usage_
(
  pointToPoint::typeName,
  "\n    Usage: pointToPoint <pointSet>\n\n"
  "    Select all points in the pointSet\n\n"
);

// Constructors 
// Construct from components
mousse::pointToPoint::pointToPoint
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::pointToPoint::pointToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set"))
{}
// Construct from Istream
mousse::pointToPoint::pointToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::pointToPoint::~pointToPoint()
{}
// Member Functions 
void mousse::pointToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all from pointSet " << setName_ << " ..." << endl;
    // Load the set
    pointSet loadedSet(mesh_, setName_);
    set.addSet(loadedSet);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all from pointSet " << setName_ << " ..." << endl;
    // Load the set
    pointSet loadedSet(mesh_, setName_);
    set.deleteSet(loadedSet);
  }
}
